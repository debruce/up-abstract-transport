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
	const std::string name;
	const AnyMap props;

	template <typename T>
	TransportTag(T&& name) : name(name) {}

	template <typename T>
	TransportTag(T&& name, const AnyMap& am) : name(name), props(am) {}

	virtual ~TransportTag() {}

	template <typename T>
	bool operator==(T&& arg) const {
		return name == arg;
	}

	template <typename T>
	T get(const std::string& n) const
	{
		return std::any_cast<T>(props.at(n));
	}
};

static const char* default_transport_doc =
    R"(
{
    "serializers": "${IMPL_SERIALIZE}",
    "Zenoh": "${IMPL_ZENOH}",
    "UdpSocket": "${IMPL_UDPSOCKET}"
}
)";

struct Transport {
	std::shared_ptr<HiddenTransport> pImpl;

	Transport(const Doc& init_doc);
	Transport(const char* init_string = default_transport_doc);
	Serializer getSerializer(const std::string&);
	std::any getConcept(const TransportTag& tag, const std::string&);
	Doc describe() const;
};

struct Message {
	std::string payload;
	std::string attributes;
};

using RpcReply = std::optional<Message>;

};  // namespace UpAbstractTransport
