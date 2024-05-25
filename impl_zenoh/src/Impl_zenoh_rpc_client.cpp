#include "UpAbstractTransport.hpp"
#include "Impl_zenoh.hpp"

namespace Impl_zenoh {

using namespace UpAbstractTransport;
using namespace std;

static string keyexpr2string(const z_keyexpr_t& keyexpr)
{
    z_owned_str_t keystr = z_keyexpr_to_string(keyexpr);
    string ret(z_loan(keystr));
    z_drop(z_move(keystr));
    return ret;    
}

template <typename T>
static z_bytes_t make_zbytes(const T& t)
{
    return {.len=t.size(), .start=(const uint8_t*)t.data()};
}

static string extract(const z_bytes_t& b)
{
    return string((const char*)b.start, b.len);
}

struct RpcClientImpl : public RpcClientApi {
    shared_ptr<TransportImpl> trans_impl;
    z_owned_reply_channel_t channel;

    RpcClientImpl(Transport transport, const string& topic, const Message& message, const chrono::milliseconds& timeout)
    {
        trans_impl = any_cast<shared_ptr<TransportImpl>>(transport.pImpl->impl);

        z_keyexpr_t keyexpr = z_keyexpr(topic.c_str());
        if (!z_check(keyexpr)) throw std::runtime_error("Not a valid key expression");
        channel = zc_reply_fifo_new(16);
        auto opts = z_get_options_default();
        auto attrs = z_bytes_map_new();
        opts.value.payload = make_zbytes(message.payload);
        z_bytes_map_insert_by_alias(&attrs, z_bytes_new("attributes"), make_zbytes(message.attributes));
        opts.attachment = z_bytes_map_as_attachment(&attrs);
        opts.timeout_ms = timeout.count();
        z_get(trans_impl->session.loan(), keyexpr, "", z_move(channel.send), &opts);
    }

    ~RpcClientImpl()
    {
        z_drop(z_move(channel));
    }

    RpcReply operator()() override
    {
        string src;
        Message message;
        z_owned_reply_t reply = z_reply_null();

        for (z_call(channel.recv, &reply); z_check(reply); z_call(channel.recv, &reply)) {
            if (z_reply_is_ok(&reply)) {
                z_sample_t sample = z_reply_ok(&reply);
                src = keyexpr2string(sample.keyexpr);
                message.payload = extract(sample.payload);
                z_bytes_t attr = z_attachment_get(sample.attachment, z_bytes_new("attributes"));
                message.attributes = extract(attr);
                break;
            } else {
                throw std::runtime_error("Received an error");
            }
        }

        z_drop(z_move(reply));
        return message;
    }
};

std::shared_ptr<RpcClientApi> rpc_client_getter(Transport transport, const std::string& topic, const Message& message, const chrono::milliseconds& timeout)
{
    return make_shared<RpcClientImpl>(transport, topic, message, timeout);
}

}; // Impl_zenoh