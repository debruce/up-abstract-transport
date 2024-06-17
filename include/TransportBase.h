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

struct TransportTag {
	std::string name;

	template <typename T>
	TransportTag(T&& name) : name(name) {}

	virtual ~TransportTag() {}

	template <typename T>
	bool operator==(T&& arg) const {
		return name == arg;
	}
};

struct Transport {
	std::shared_ptr<HiddenTransport> pImpl;

	Transport(const Doc& init_doc);
	Transport(const char* init_string);
	Serializer getSerializer(const std::string&);
	std::any getConcept(const TransportTag& tag, const std::string&);
	// std::vector<std::string> listConcepts();
};

struct Message {
	std::string payload;
	std::string attributes;
};

using RpcReply = std::optional<Message>;

};  // namespace UpAbstractTransport
