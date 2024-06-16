#pragma once

#include <any>
#include <chrono>
#include <functional>
#include <future>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "Serializer.h"

namespace UpAbstractTransport {
using Doc = nlohmann::json;

struct HiddenTransport;
struct Serializer;

struct Transport {
	std::shared_ptr<HiddenTransport> pImpl;

	Transport(const Doc& init_doc);
	Transport(const char* init_string);
	std::any getConcept(const std::string&);
	std::vector<std::string> listConcepts();
	Serializer getSerializer(const std::string&);
};

struct Message {
	std::string payload;
	std::string attributes;
};

using RpcReply = std::optional<Message>;
};  // namespace UpAbstractTransport
