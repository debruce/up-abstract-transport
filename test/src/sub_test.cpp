#include <unistd.h>

#include <iostream>

#include "UpAbstractTransport.h"

using namespace std;
using namespace UpAbstractTransport;

int main(int argc, char* argv[]) {
	auto transport = Transport();
	auto callback = [&](const string& sending_topic,
	                    const string& listening_topic, const Message& message) {
		auto attributes = transport.getSerializer("UAttributes");
		attributes.deserialize(message.attributes);
		cout << "subscriber callback with"
		     << " from=" << sending_topic << " to=" << listening_topic
		     << " payload=" << message.payload
		     << " attributes=" << attributes.debugString() << endl;
	};
	// TransportTag tag("Zenoh");
	TransportTag tag("UdpSocket", { {"ipv4", "0.0.0.0"}, {"port", 4444}}); 
	auto subscriber = Subscriber(transport, "upl/*", callback, tag);
	sleep(10000);
}
