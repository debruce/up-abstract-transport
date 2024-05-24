#pragma once

#include <memory>
#include <any>
#include <string>
#include <chrono>
#include <optional>
#include <future>
#include <nlohmann/json.hpp>

namespace UpAbstractTransport {
    using Doc = nlohmann::json;

    struct Transport {
        Transport(const Doc& init_doc);
        Transport(const char* init_string);
        std::any get_factory(const std::string&);

        struct Impl;
        std::shared_ptr<Impl>   pImpl;
    };

    struct Transport::Impl {
        std::any    impl;
        std::map<std::string, std::any> getters;

        Impl(const Doc& init_doc);
        std::any get_factory(const std::string& name);

    };
    struct Message {
        std::string payload;
        std::string attributes;
    };

    using SubscriberCallback = std::function<
        void (const std::string& sending_topic, const std::string& listening_topic, const Message&)>;
    
    using RpcReply = std::optional<Message>;
    using RpcServerCallback = std::function<
        RpcReply (const std::string& sending_topic, const std::string& listening_topic, const Message&)>;

    struct PublisherApi {
        typedef std::shared_ptr<PublisherApi> (*Getter)(Transport, const std::string&);
        virtual void operator()(const Message&) = 0;
    };

    class Publisher {
        std::shared_ptr<PublisherApi> pImpl;
        public:
        Publisher(Transport transport, const std::string& topic);
        void operator()(const Message& message) { (*pImpl)(message); }
    };


    struct SubscriberApi {
        typedef std::shared_ptr<SubscriberApi> (*Getter)(Transport, const std::string&, SubscriberCallback);
    };

    class Subscriber {
        std::shared_ptr<SubscriberApi> pImpl;
    public:
        Subscriber(Transport transport, const std::string& topic, SubscriberCallback callback);
    };


    struct RpcClientApi {
        typedef std::shared_ptr<RpcClientApi> (*Getter)(Transport, const std::string&, const Message&, const std::chrono::seconds&);
        virtual RpcReply operator()() = 0;
    };

    class RpcClient {
        std::shared_ptr<RpcClientApi> pImpl;
    public:
        RpcClient(Transport transport, const std::string& topic, const Message& message, const std::chrono::seconds& timeout);

        RpcReply operator()() { return (*pImpl)(); }
    };

    std::future<RpcReply> queryCall(Transport transport, std::string expr, const Message& message, const std::chrono::seconds& timeout);


    struct RpcServerApi {
        typedef std::shared_ptr<RpcServerApi> (*Getter)(Transport, const std::string&, RpcServerCallback);
    };

    class RpcServer {
        std::shared_ptr<RpcServerApi> pImpl;
    public:
        RpcServer(Transport transport, const std::string& topic, RpcServerCallback callback);
    };

};

