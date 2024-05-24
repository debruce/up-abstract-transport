#include "UpAbstractTransport.hpp"
#include "zenohc.hxx"
#include <nlohmann/json.hpp>
#include <iostream>

namespace Impl_zenoh {

using namespace UpAbstractTransport;
using namespace std;

template <typename T>
static z_bytes_t make_zbytes(const T& t)
{
    return {.len=t.size(), .start=(const uint8_t*)t.data()};
}

static zenohc::Session inst()
{
    zenohc::Config config;
    return zenohc::expect<zenohc::Session>(zenohc::open(std::move(config)));
}

///////////////////////////////////////////////////////////////////////////////////
// TransportImpl
///////////////////////////////////////////////////////////////////////////////////
struct TransportImpl {
    zenohc::Session session;

    TransportImpl(const nlohmann::json& doc) : session(inst())
    {
        cout << __PRETTY_FUNCTION__ << " doc=" << doc << endl; 
    }

    ~TransportImpl()
    {
        cout << __PRETTY_FUNCTION__ << endl; 
    }
};

any transport_getter(const nlohmann::json& doc)
{
    return make_shared<TransportImpl>(doc);
}

///////////////////////////////////////////////////////////////////////////////////
// Publisher
///////////////////////////////////////////////////////////////////////////////////
struct PublisherImpl : public PublisherApi {
    shared_ptr<TransportImpl> trans_impl;
    z_owned_publisher_t handle;

    PublisherImpl(Transport transport, const std::string& expr)
    {
        trans_impl = any_cast<shared_ptr<TransportImpl>>(transport.pImpl->impl);
        handle = z_declare_publisher(trans_impl->session.loan(), z_keyexpr(expr.c_str()), nullptr);
        if (!z_check(handle)) throw std::runtime_error("Cannot declare publisher");
    }

    ~PublisherImpl()
    {
        z_undeclare_publisher(&handle);
    }

    void operator()(const Message& msg) override
    {
        z_publisher_put_options_t options = z_publisher_put_options_default();
        z_owned_bytes_map_t map = z_bytes_map_new();
        options.attachment = z_bytes_map_as_attachment(&map);
        z_bytes_map_insert_by_alias(&map, z_bytes_new("attributes"), make_zbytes(msg.attributes));
        if (z_publisher_put(z_loan(handle), (const uint8_t*)msg.payload.data(), msg.payload.size(), &options)) {
            z_drop(z_move(map));
            throw std::runtime_error("Cannot publish");
        }
        z_drop(z_move(map));
    }
};

std::shared_ptr<PublisherApi> publisher_getter(Transport transport, const std::string& name)
{
    return make_shared<PublisherImpl>(transport, name);
}

///////////////////////////////////////////////////////////////////////////////////
// Subscriber
///////////////////////////////////////////////////////////////////////////////////
struct SubscriberImpl : public SubscriberApi {
    SubscriberImpl(Transport transport, const std::string& topic, SubscriberServerCallback)
    {
        cout << __PRETTY_FUNCTION__ << " topic=" << topic << endl; 
    }
};

std::shared_ptr<SubscriberApi> subscriber_getter(Transport transport, const std::string& topic, SubscriberServerCallback callback)
{
    return make_shared<SubscriberImpl>(transport, topic, callback);
}

///////////////////////////////////////////////////////////////////////////////////
// RpcClient
///////////////////////////////////////////////////////////////////////////////////
struct RpcClientImpl : public RpcClientApi {
    RpcClientImpl(Transport transport, const std::string& topic, const Message& message, const std::chrono::seconds& timeout)
    {
        cout << __PRETTY_FUNCTION__ << " topic=" << topic << endl; 
    }

    tuple<string, Message> operator()() override
    {
        cout << __PRETTY_FUNCTION__ << endl;
        return make_tuple(string("hello"), Message{});      
    }
};

std::shared_ptr<RpcClientApi> rpc_client_getter(Transport transport, const std::string& topic, const Message& message, const std::chrono::seconds& timeout)
{
    return make_shared<RpcClientImpl>(transport, topic, message, timeout);
}

///////////////////////////////////////////////////////////////////////////////////
// RpcServer
///////////////////////////////////////////////////////////////////////////////////
struct RpcServerImpl : public RpcServerApi {
    RpcServerImpl(Transport transport, const string& topic, RpcServerCallback callback)
    {
        cout << __PRETTY_FUNCTION__ << " topic=" << topic << endl; 
    }
};

std::shared_ptr<RpcServerApi> rpc_server_getter(Transport transport, const string& topic, RpcServerCallback callback)
{
    return make_shared<RpcServerImpl>(transport, topic, callback);
}

}; // Impl_zenoh