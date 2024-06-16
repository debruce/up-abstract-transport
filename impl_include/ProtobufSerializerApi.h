#pragma once

#include <google/protobuf/message.h>

#include "Serializer.h"

namespace UpAbstractTransport {
struct ProtobufSerializerApi : public SerializerApi {
	google::protobuf::Message* msg_ptr;

	~ProtobufSerializerApi();

	virtual std::string validate() const;
	std::string messageName() const override;
	std::string debugString() const override;
	std::string serialize() const override;
	bool deserialize(const std::string&) override;
	bool assign(const AnyMap& arg) override;
	AnyMap fetch(bool describe) const override;
};

};  // namespace UpAbstractTransport