#pragma once

#include <memory>
#include <string_view>
#include <variant>
#include <optional>
#include <functional>
#include <future>
#include <nlohmann/json.hpp>

namespace UpAbstractTransport {
    using Doc = nlohmann::json;
    using PayloadView = std::string_view;

    struct Transport {
        Transport(const Doc& init_doc);
        Transport(const char* init_string);

        struct Impl;
        std::shared_ptr<Impl>   pImpl;
    };

    struct UUri {};
    struct Message {};
    struct PayloadFormat {}; // enum from protobuf
    struct Priority {}; // enum from protobuf
    struct Code {}; // enum from protobuf
    struct Status {}; // protobuf message including UCode enum, optional string, and protobuf any
    using Ttl = std::chrono::milliseconds;
    using PermissionLevel = uint32_t;

    struct Publisher {
        Publisher(
            Transport transport,
            const UUri& topic,
            const PayloadFormat& format,
            std::optional<Priority> priority = {},
            std::optional<Ttl> ttl = {}
            );
        Status publish(const PayloadView&);

        struct Impl;
        std::shared_ptr<Impl>   pImpl;
    };

    struct Subscriber {
        Subscriber(
            Transport transport,
            const UUri& topic,
            std::function<void (const Message&)> callback
            );

        struct Impl;
        std::shared_ptr<Impl>   pImpl;
    };

    // struct NotificationSource {
    //     NotificationSource(
    //         Transport transport,
    //         const UUri& source,
    //         const UUri& sink,
    //         const PayloadFormat& format,
    //         std::optional<Priority> priority = {},
    //         std::optional<Ttl> ttl = {}
    //         );
    //     Status notify(const PayloadView&);

    //     struct Impl;
    //     std::shared_ptr<Impl>   pImpl;
    // };

    // struct NotificationSink {
    //     NotificationSink(
    //         Transport transport,
    //         const UUri& topic,
    //         std::function<void (const Message&)> callback
    //         );

    //     struct Impl;
    //     std::shared_ptr<Impl>   pImpl;
    // };

    struct RpcClient {
        using MessageOrStatus = std::variant<Message, std::variant<Status, Code>>;
        RpcClient(
            Transport transport,
            const UUri& method,
            const Priority& priority,
            const Ttl& ttl,
            std::optional<PayloadFormat> format = {},
            std::optional<PermissionLevel> permission_level = {},
            std::optional<std::string> token = {}
            );

        std::future<MessageOrStatus> invokeMethod(const PayloadView&);
        std::future<MessageOrStatus> invokeMethod();

        struct Impl;
        std::shared_ptr<Impl>   pImpl;
    };

    struct RpcServer {
        using MessageOrStatus = std::variant<Message, std::variant<Status, Code>>;
        RpcServer(
            Transport transport,
            const UUri& method,
            std::function<std::optional<Message> (const Message&)> callback,
            std::optional<PayloadFormat> format,
            std::optional<Ttl> ttl
            );

        struct Impl;
        std::shared_ptr<Impl>   pImpl;
    };
};

