#include "UpAbstractTransport.hpp"
#include <map>
#include <any>
#include <iostream>

namespace UpAbstractTransport {

using namespace std;


Publisher::Publisher(Transport transport, const std::string& topic)
{
   auto a = transport.get_factory("publisher");
   auto getter = any_cast<PublisherApi::Getter>(a);
   pImpl = (*getter)(transport, topic);
}


Subscriber::Subscriber(Transport transport, const std::string& topic, SubscriberCallback callback)
{
   auto a = transport.get_factory("subscriber");
   auto getter = any_cast<SubscriberApi::Getter>(a);
   pImpl = (*getter)(transport, topic, callback);      
}


RpcClient::RpcClient(Transport transport, const std::string& topic, const Message& message, const std::chrono::seconds& timeout)
{
   auto a = transport.get_factory("rpc_client");
   auto getter = any_cast<RpcClientApi::Getter>(a);
   pImpl = (*getter)(transport, topic, message, timeout);       
}

future<tuple<string, Message>> queryCall(Transport transport, string expr, const Message& message, const chrono::seconds& timeout)
{
    auto msg = make_shared<Message>(message);
    return async([=]() { return RpcClient(transport, expr, *msg, timeout)(); } );
}


RpcServer::RpcServer(Transport transport, const std::string& topic, RpcServerCallback callback)
{
   auto a = transport.get_factory("rpc_server");
   auto getter = any_cast<RpcServerApi::Getter>(a);
   pImpl = (*getter)(transport, topic, callback);        
}

}; // UpAbstractTransport

namespace Impl_zenoh {
   using namespace UpAbstractTransport;
   any transport_getter(const nlohmann::json& doc);
   std::shared_ptr<PublisherApi> publisher_getter(Transport transport, const std::string& name);
   std::shared_ptr<SubscriberApi> subscriber_getter(Transport transport, const std::string& topic, SubscriberCallback callback);
   std::shared_ptr<RpcClientApi> rpc_client_getter(Transport transport, const std::string& topic, const Message& message, const std::chrono::seconds& timeout);
   std::shared_ptr<RpcServerApi> rpc_server_getter(Transport transport, const std::string& topic, RpcServerCallback callback);
};

namespace UpAbstractTransport {

using namespace std;

Transport::Transport(const Doc& init_doc)
    : pImpl(new Impl(init_doc))
{
}

Transport::Transport(const char* init_string)
    : pImpl(new Impl(Doc::parse(init_string)))
{
}

any Transport::get_factory(const string& name)
{
    return pImpl->get_factory(name);
}

Transport::Impl::Impl(const Doc& init_doc) {
    auto transport_style = init_doc["transport"].get<string>();
    cout << "transport_style = " << transport_style << endl;

    if (transport_style == "zenoh") {
        impl = ::Impl_zenoh::transport_getter(init_doc);
        getters["publisher"] = ::Impl_zenoh::publisher_getter;
        getters["subscriber"] = ::Impl_zenoh::subscriber_getter;
        getters["rpc_client"] = ::Impl_zenoh::rpc_client_getter;
        getters["rpc_server"] = ::Impl_zenoh::rpc_server_getter;
    }
    else throw runtime_error("Unsupported transport");
}

any Transport::Impl::get_factory(const string& name)
{
    return getters[name];
}

}; // namespace UpAbstractTransport