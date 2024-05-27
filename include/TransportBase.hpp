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

    struct HiddenTransport;
    struct SerializerApi;

    struct Transport
    {
        std::shared_ptr<HiddenTransport> pImpl;

        Transport(const Doc &init_doc);
        Transport(const char *init_string);
        std::any get_concept(const std::string &);
        std::shared_ptr<SerializerApi> get_serializer(const std::string &);
    };

    struct ConceptFactories
    {
        std::function<std::shared_ptr<ConceptApi>(const Doc &init_doc)> get_impl;
    };

    struct SerializerFactories
    {
        std::function<std::shared_ptr<SerializerApi>(const std::string& kind)> get_instance;
    };

    struct Message
    {
        std::string payload;
        std::string attributes;
    };

    using RpcReply = std::optional<Message>;
};
