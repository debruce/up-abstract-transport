#include <unistd.h>

#include <algorithm>
#include <iostream>

#include "UpAbstractTransport.h"

using namespace std;
using namespace UpAbstractTransport;

int main(int argc, char* argv[]) {
	auto transport = Transport();
	auto callback = [&](const string& sending_topic,
	                   const string& listening_topic,
	                   const Message& message) -> RpcReply {
		auto uattributes = transport.getSerializer("UAttributes");
		uattributes.deserialize(message.attributes);
		cout << "rpc callback with"
		     << " from=" << sending_topic << " to=" << listening_topic
		     << " payload=" << message.payload
		     << " attributes=" << uattributes.debugString() << endl;
		string upayload;
		for (auto c : message.payload)
			upayload += std::toupper(c);
		return Message{.payload=upayload, .attributes=uattributes.serialize()};
	};
	// TransportTag tag("Zenoh");
	TransportTag tag("UdpSocket", { {"ipv4", "0.0.0.0"}, {"port", 4444}}); 
	auto rpc_server = RpcServer(transport, "demo/rpc/*", callback, tag);
	sleep(10000);
}
