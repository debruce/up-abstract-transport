#include <unistd.h>

#include <iostream>

#include "UpAbstractTransport.h"

using namespace std;
using namespace UpAbstractTransport;

template <class... Args>
string genString(const char* fmt, Args... args) {
	char buf[128];
	snprintf(buf, sizeof(buf), fmt, args...);
	return string(buf);
}

AnyMap idHelper() {
	uint64_t timestamp =
	    std::chrono::duration_cast<std::chrono::milliseconds>(
	        std::chrono::system_clock::now().time_since_epoch())
	        .count();
	// uint32_t msb = (timestamp << 16) | (8ULL << 12) | (0x123ULL);
	// uint32_t lsb = (2ULL << 62) | (0xFFFFFFFFFFFFULL);
	auto msb = 5;
	auto lsb = 6;
	return AnyMap{{"lsb", lsb}, {"msb", msb}};
}

int main(int argc, char* argv[]) {
	auto transport = Transport();
	// TransportTag tag("Zenoh");
	TransportTag tag("UdpSocket", { {"ipv4", "0.0.0.0"}, {"port", 4444}});

	for (auto i = 0; i < 5; i++) {
		auto uattributes = transport.getSerializer("UAttributes");
		AnyMap am{{"commstatus", "DEADLINE_EXCEEDED"},
				{"permission_level", 7},
				{"token", "token_tester"},
				{"traceparent", "traceparent_tester"},
				{"ttl", 8},
				{"payload_format", "UPAYLOAD_FORMAT_PROTOBUF_WRAPPED_IN_ANY"},
				{"priority", "UPRIORITY_CS1"},
				{"sink", AnyMap{{"authority_name", "aname1"},
								{"resource_id", 30},
								{"ue_id", 31},
								{"ue_version_major", 32}}},
				{"source", AnyMap{{"authority_name", "aname2"},
									{"resource_id", 40},
									{"ue_id", 41},
									{"ue_version_major", 42}}},
				{"type", "UMESSAGE_TYPE_RESPONSE"}};
		am["id"] = idHelper();
		am["reqid"] = am["id"];
		uattributes.assign(am);
		auto f = rpcCallFuture(
		    transport, "demo/rpc/action1",
		    Message{.payload=genString("pay_A_%d", i), .attributes = uattributes.serialize()},
		    chrono::seconds(2), tag);
			cout << "after rpcCallFuture" << endl;
		auto result = f.get();
		uattributes.deserialize(result->attributes);
		cout << "got result payload=" << result->payload
		     << " attributes=" << uattributes.debugString() << endl;
		usleep(100000);
	}
}
