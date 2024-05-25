#include "UpAbstractTransport.hpp"
#include "Impl_zenoh.hpp"
#include <mutex>
#include <condition_variable>
#include <deque>
#include <thread>

namespace Impl_zenoh {

using namespace UpAbstractTransport;
using namespace std;

static std::string keyexpr2string(const z_keyexpr_t& keyexpr)
{
    z_owned_str_t keystr = z_keyexpr_to_string(keyexpr);
    std::string ret(z_loan(keystr));
    z_drop(z_move(keystr));
    return ret;    
}

static string stringFromZBytes(const z_bytes_t& z)
{
    return string((const char*)z.start, z.len);
}

static z_bytes_t zbytesFromOpaque(const string_view& od)
{
    return z_bytes_t{.len = od.size(), .start = (uint8_t*)od.data()};
}

struct QInfoImpl {
    string source;
    Message message;
    z_owned_query_t owned_query;

    QInfoImpl(const z_query_t *query)
    {
        source = keyexpr2string(z_query_keyexpr(query));
        z_bytes_t pred = z_query_parameters(query);
        z_value_t value = z_query_value(query);
        message.payload = stringFromZBytes(value.payload);

        z_attachment_t attachment = z_query_attachment(query);
        if (!z_check(attachment)) throw std::runtime_error("attachment is missing");
        z_bytes_t avalue = z_attachment_get(attachment, z_bytes_new("attributes"));
        message.attributes = stringFromZBytes(avalue);
        owned_query = z_query_clone(query);
    }


    ~QInfoImpl()
    {
        z_query_drop(&owned_query);
    }

    void reply(const Message& message, const string& expr)
    {
        auto query = z_query_loan(&owned_query);
        z_query_reply_options_t options = z_query_reply_options_default();
        z_owned_bytes_map_t map = z_bytes_map_new();
        z_bytes_map_insert_by_alias(&map, z_bytes_new("attributes"), zbytesFromOpaque(message.attributes));
        options.attachment = z_bytes_map_as_attachment(&map);
        z_query_reply(&query, z_keyexpr(expr.c_str()), (const uint8_t*)message.payload.data(), message.payload.size(), &options);
    } 
};

struct RpcServerImpl : public RpcServerApi {
    shared_ptr<TransportImpl> trans_impl;
    z_owned_queryable_t qable;
    string listening_topic;
    zenohc::KeyExprView expr;
    std::mutex  mtx;
    std::condition_variable cv;
    std::deque<std::shared_ptr<QInfoImpl>> queue;
    bool die;
    RpcServerCallback callback;

    std::vector<std::thread> thread_pool;

    static void _handler(const z_query_t *query, void *context)
    {
        auto me = reinterpret_cast<RpcServerImpl*>(context);
        me->handler(query);
    }

    virtual void handler(const z_query_t *query)
    {
        auto wq = std::make_shared<QInfoImpl>(query);
        std::unique_lock<std::mutex> lock(mtx);
        queue.push_front(wq);
        cv.notify_one();
    }

    void worker()
    {
        using namespace std;

        while (true) {
            shared_ptr<QInfoImpl> ptr;
            {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [&](){ return !queue.empty() && !die; });
                if (die) break;
                ptr = queue.back();
                queue.pop_back();
            }
            auto result = callback(ptr->source, listening_topic, ptr->message);
            if (result) {
                ptr->reply(*result, listening_topic);
            }
        }
    }

    RpcServerImpl(Transport transport, const string& topic, RpcServerCallback _callback) : expr(topic)
    {
        trans_impl = any_cast<shared_ptr<TransportImpl>>(transport.pImpl->impl);
        listening_topic = topic;
        callback = _callback;

        z_owned_closure_query_t closure = z_closure(_handler, NULL, this);
        qable = z_declare_queryable(trans_impl->session.loan(), expr, z_move(closure), NULL);
        if (!z_check(qable)) throw std::runtime_error("Unable to create queryable.");

        die = false;
        const size_t thread_count = 4;
        thread_pool.reserve(thread_count);
        for (size_t i = 0; i < 10; i++) {
            thread_pool.emplace_back([&]() { worker(); });
        }
    }

    ~RpcServerImpl()
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            die = true;
        }
        for (auto& thr : thread_pool) thr.join();
        thread_pool.clear();

        z_undeclare_queryable(z_move(qable));        
    }
};

std::shared_ptr<RpcServerApi> rpc_server_getter(Transport transport, const string& topic, RpcServerCallback callback)
{
    return make_shared<RpcServerImpl>(transport, topic, callback);
}

}; // Impl_zenoh