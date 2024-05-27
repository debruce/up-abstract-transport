#include "HiddenTransport.hpp"
#include <map>
#include <any>
#include <filesystem>
#include <iostream>
#include <wordexp.h>

namespace UpAbstractTransport
{
    using namespace std;

    Serializer::Serializer(Transport transport, const string &kind)
    {
        pImpl = transport.get_serializer(kind);
    }

    std::string Serializer::hello(const string &arg)
    {
        return pImpl->hello(arg);
    }

    Publisher::Publisher(Transport transport, const string &topic)
    {
        auto a = transport.get_concept("publisher");
        auto getter = any_cast<PublisherApi::Getter>(a);
        pImpl = (*getter)(transport, topic);
    }

    Subscriber::Subscriber(Transport transport, const string &topic, SubscriberCallback callback)
    {
        auto a = transport.get_concept("subscriber");
        auto getter = any_cast<SubscriberApi::Getter>(a);
        pImpl = (*getter)(transport, topic, callback);
    }

    RpcClient::RpcClient(Transport transport, const string &topic, const Message &message, const chrono::milliseconds &timeout)
    {
        auto a = transport.get_concept("rpc_client");
        auto getter = any_cast<RpcClientApi::Getter>(a);
        pImpl = (*getter)(transport, topic, message, timeout);
    }

    future<RpcReply> rpcCall(Transport transport, const string &topic, const Message &message, const chrono::milliseconds &timeout)
    {
        auto topicCopy = make_shared<string>(topic);
        auto msg = make_shared<Message>(message);
        return async([=]()
                     { return RpcClient(transport, *topicCopy, *msg, timeout)(); });
    }

    RpcServer::RpcServer(Transport transport, const string &topic, RpcServerCallback callback)
    {
        auto a = transport.get_concept("rpc_server");
        auto getter = any_cast<RpcServerApi::Getter>(a);
        pImpl = (*getter)(transport, topic, callback);
    }

    string resolve_path(const string &impl)
    {
        wordexp_t we;
        auto retval = wordexp(impl.c_str(), &we, WRDE_UNDEF);
        if (retval != 0)
        {
            wordfree(&we);
            throw runtime_error("wordexp expansion failed");
        }
        if (we.we_wordc != 1)
        {
            wordfree(&we);
            throw runtime_error("wordexp expanded to more than one path");
        }
        string result(we.we_wordv[0]);
        wordfree(&we);
        return result;
    }

    HiddenTransport::HiddenTransport(const Doc &init_doc)
    {
        string path;

        path = resolve_path(init_doc["implementation"].get<string>());
        conceptPlugin = FactoryPlugin<ConceptFactories>(path);
        conceptImpl = conceptPlugin->get_impl(init_doc);

        path = resolve_path(init_doc["serializers"].get<string>());
        serialPlugin = FactoryPlugin<SerializerFactories>(path);
    }

    Transport::Transport(const Doc &init_doc) : pImpl(new HiddenTransport(init_doc))
    {
    }

    Transport::Transport(const char *init_string) : Transport(Doc::parse(init_string))
    {
    }

    any Transport::get_concept(const string &name)
    {
        return pImpl->conceptImpl->get_factory(name);
    }

    shared_ptr<SerializerApi> Transport::get_serializer(const string &name)
    {
        return pImpl->serialPlugin->get_instance(name);
    }

}; // namespace UpAbstractTransport