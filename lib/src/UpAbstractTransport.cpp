#include "UpAbstractTransport.hpp"
#include "FactoryPlugin.hpp"
#include <map>
#include <any>
#include <filesystem>
#include <iostream>
#include <wordexp.h>

namespace UpAbstractTransport {

using namespace std;


Publisher::Publisher(Transport transport, const string& topic)
{
   auto a = transport.get_factory("publisher");
   auto getter = any_cast<PublisherApi::Getter>(a);
   pImpl = (*getter)(transport, topic);
}


Subscriber::Subscriber(Transport transport, const string& topic, SubscriberCallback callback)
{
   auto a = transport.get_factory("subscriber");
   auto getter = any_cast<SubscriberApi::Getter>(a);
   pImpl = (*getter)(transport, topic, callback);      
}


RpcClient::RpcClient(Transport transport, const string& topic, const Message& message, const chrono::milliseconds& timeout)
{
   auto a = transport.get_factory("rpc_client");
   auto getter = any_cast<RpcClientApi::Getter>(a);
   pImpl = (*getter)(transport, topic, message, timeout);       
}

future<RpcReply> queryCall(Transport transport, const string& topic, const Message& message, const chrono::milliseconds& timeout)
{
    auto topicCopy = make_shared<string>(topic);
    auto msg = make_shared<Message>(message);
    return async([=]() { return RpcClient(transport, *topicCopy, *msg, timeout)(); } );
}


RpcServer::RpcServer(Transport transport, const string& topic, RpcServerCallback callback)
{
   auto a = transport.get_factory("rpc_server");
   auto getter = any_cast<RpcServerApi::Getter>(a);
   pImpl = (*getter)(transport, topic, callback);        
}

}; // UpAbstractTransport

namespace UpAbstractTransport {

using namespace std;

string resolve_path(const string& impl)
{
    wordexp_t   we;
    auto retval = wordexp(impl.c_str(), &we, WRDE_UNDEF);
    if (retval != 0) {
        wordfree(&we);
        throw runtime_error("wordexp expansion failed");
    }
    if (we.we_wordc != 1) {
        wordfree(&we);
        throw runtime_error("wordexp expanded to more than one path");
    }
    string result(we.we_wordv[0]);
    wordfree(&we);
    return result;
}

Transport::Transport(const Doc& init_doc)
{
    auto transport_style = init_doc["transport"].get<string>();

    if (transport_style == "zenoh") {
        auto realpath = resolve_path(init_doc["implementation"].get<string>());
        auto plugin = FactoryPlugin<Factories>(realpath);
        pImpl = plugin->get_transport_impl(init_doc);
    }
    else throw runtime_error("Unsupported transport");
}

Transport::Transport(const char* init_string) : Transport(Doc::parse(init_string))
{}

any Transport::get_factory(const string& name)
{
    return pImpl->get_factory(name);
}

}; // namespace UpAbstractTransport