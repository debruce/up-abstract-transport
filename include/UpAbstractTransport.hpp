#pragma once

#include <memory>
#include <any>
#include <string>
#include <chrono>
#include <optional>
#include <functional>
#include <future>
#include <nlohmann/json.hpp>

namespace UpAbstractTransport
{
    using Doc = nlohmann::json;

    struct ConceptApi
    {
        virtual std::any get_factory(const std::string &) = 0;
    };

    struct SerialApi
    {
        virtual std::any get_factory(const std::string &) = 0;
    };

    struct HiddenTransport;

    struct Transport
    {
        std::shared_ptr<HiddenTransport> pImpl;

        Transport(const Doc &init_doc);
        Transport(const char *init_string);
        std::any get_concept(const std::string &);
        std::any get_serializer(const std::string &);
    };

    // class Serial
    // {
    //     std::shared_ptr<SerialApi> pImpl;

    // public:
    //     Serial(Transport, const std::string &);
    //     std::string hello(const std::string &arg) { return pImpl->hello(arg); }
    // };

    struct Message
    {
        std::string payload;
        std::string attributes;
    };

    using SubscriberCallback = std::function<
        void(const std::string &sending_topic, const std::string &listening_topic, const Message &)>;

    using RpcReply = std::optional<Message>;
    using RpcServerCallback = std::function<
        RpcReply(const std::string &sending_topic, const std::string &listening_topic, const Message &)>;

    struct PublisherApi
    {
        typedef std::shared_ptr<PublisherApi> (*Getter)(Transport, const std::string &);
        virtual void operator()(const Message &) = 0;
    };

    class Publisher
    {
        std::shared_ptr<PublisherApi> pImpl;

    public:
        Publisher(Transport, const std::string &);
        void operator()(const Message &message) { (*pImpl)(message); }
    };

    struct SubscriberApi
    {
        typedef std::shared_ptr<SubscriberApi> (*Getter)(Transport, const std::string &, SubscriberCallback);
    };

    class Subscriber
    {
        std::shared_ptr<SubscriberApi> pImpl;

    public:
        Subscriber(Transport, const std::string &, SubscriberCallback);
    };

    struct RpcClientApi
    {
        typedef std::shared_ptr<RpcClientApi> (*Getter)(Transport, const std::string &, const Message &, const std::chrono::milliseconds &);
        virtual RpcReply operator()() = 0;
    };

    class RpcClient
    {
        std::shared_ptr<RpcClientApi> pImpl;

    public:
        RpcClient(Transport, const std::string &, const Message &, const std::chrono::milliseconds &);

        RpcReply operator()() { return (*pImpl)(); }
    };

    std::future<RpcReply> queryCall(Transport, const std::string &, const Message &, const std::chrono::milliseconds &);

    struct RpcServerApi
    {
        typedef std::shared_ptr<RpcServerApi> (*Getter)(Transport, const std::string &, RpcServerCallback);
    };

    class RpcServer
    {
        std::shared_ptr<RpcServerApi> pImpl;

    public:
        RpcServer(Transport, const std::string &, RpcServerCallback);
    };

    struct ConceptFactories
    {
        std::function<std::shared_ptr<ConceptApi>(const Doc &init_doc)> get_impl;
    };

    struct SerializerFactories
    {
        std::function<std::shared_ptr<SerialApi>()> get_impl;
    };
};
