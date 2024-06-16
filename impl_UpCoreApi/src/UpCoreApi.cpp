#include <sstream>

#include "FactoryPlugin.h"
#include "HiddenTransport.h"
#include "ProtobufSerializerApi.h"
#include "uprotocol/v1/uattributes.pb.h"

using namespace UpAbstractTransport;
using namespace std;

string removeNewlines(const string& arg) {
	string ret;
	ret.reserve(arg.size());
	for (auto c : arg) {
		if (c == '\n')
			ret += ' ';
		else
			ret += c;
	}
	return ret;
}
namespace Impl_UpCoreApi {
struct Impl_UAttributes : public UpAbstractTransport::ProtobufSerializerApi {
	uprotocol::v1::UAttributes* uattr;

	Impl_UAttributes() {
		uattr = new uprotocol::v1::UAttributes();
		msg_ptr = uattr;
	}

	string validate() const override {
		cout << "############### validate uattributes ###############" << endl;
		// cout << "uuri = " << uattr->id().lsb() << ' ' << uattr->id().msb() <<
		// endl; cout << "type = " << uattr->type() << endl; cout << "priority =
		// " << uattr->priority() << endl;
		cout << removeNewlines(uattr->DebugString()) << endl;
		return string();
	}
};

struct Impl_UStatus : public UpAbstractTransport::ProtobufSerializerApi {
	Impl_UStatus() { msg_ptr = new uprotocol::v1::UStatus(); }
};

struct Impl_UUri : public UpAbstractTransport::ProtobufSerializerApi {
	Impl_UUri() { msg_ptr = new uprotocol::v1::UUri(); }
};

static shared_ptr<UpAbstractTransport::SerializerApi> getInstance(
    const string& kind) {
	if (kind == "UAttributes")
		return make_shared<Impl_UAttributes>();
	else if (kind == "UStatus")
		return make_shared<Impl_UStatus>();
	else if (kind == "UUri")
		return make_shared<Impl_UUri>();
	else {
		stringstream ss;
		ss << "UpCoreApi plugin does not implement \"" << kind << "\"";
		throw runtime_error(ss.str());
	}
}
};  // namespace Impl_UpCoreApi

UpAbstractTransport::SerializerFactories factories = {
    Impl_UpCoreApi::getInstance};

FACTORY_EXPOSE(factories);
