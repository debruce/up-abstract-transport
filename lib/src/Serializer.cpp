#include "AnyMap.h"
#include "HiddenTransport.h"
#include "ProtobufSerializerApi.h"

namespace UpAbstractTransport {
using namespace std;

string Serializer::messageName() const { return pImpl->messageName(); }

string Serializer::debugString() const { return pImpl->debugString(); }

string Serializer::serialize() const { return pImpl->serialize(); }

bool Serializer::deserialize(const std::string& arg) {
	return pImpl->deserialize(arg);
}

bool Serializer::assign(const AnyMap& arg) { return pImpl->assign(arg); }

AnyMap Serializer::fetch(bool describe) const { return pImpl->fetch(describe); }

/////////////////

ProtobufSerializerApi::~ProtobufSerializerApi() { delete msg_ptr; }

string ProtobufSerializerApi::messageName() const {
	return msg_ptr->GetTypeName();
}

string ProtobufSerializerApi::debugString() const {
	return msg_ptr->DebugString();
}

string ProtobufSerializerApi::validate() const {
	return string();  // default is to validate, non-empty string means good
}

string ProtobufSerializerApi::serialize() const {
	string v = validate();
	if (v.size() > 0) {
		throw runtime_error(v);
	}
	return msg_ptr->SerializeAsString();
}

bool ProtobufSerializerApi::deserialize(const string& arg) {
	return msg_ptr->ParseFromString(arg);
}

bool ProtobufSerializerApi::assign(const AnyMap& arg) {
	Anymap2Protobuf s(arg, *msg_ptr);
	return s.is_valid();
}

AnyMap ProtobufSerializerApi::fetch(bool describe) const {
	return protobuf2anymap(*msg_ptr, describe);
}

};  // namespace UpAbstractTransport