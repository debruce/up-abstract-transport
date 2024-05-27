#pragma once

#include <memory>
#include <any>
#include <string>
#include <chrono>
#include <optional>
#include <functional>
#include <future>
#include <nlohmann/json.hpp>

#include "Serializer.hpp"

namespace UpAbstractTransport
{
    using Doc = nlohmann::json;

    struct HiddenTransport;
    struct Serializer;

    struct Transport
    {
        std::shared_ptr<HiddenTransport> pImpl;

        Transport(const Doc &init_doc);
        Transport(const char *init_string);
        std::any getConcept(const std::string &);
        std::vector<std::string> listConcepts();
        Serializer getSerializer(const std::string &);
    };

    struct Message
    {
        std::string payload;
        std::string attributes;
    };

    using RpcReply = std::optional<Message>;
};
