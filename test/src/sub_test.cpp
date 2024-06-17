#include <unistd.h>

#include <iostream>

#include "UpAbstractTransport.h"

using namespace std;
using namespace UpAbstractTransport;

auto init_doc =
    R"(
{
    "Zenoh": "${IMPL_ZENOH}",
    "serializers": "${IMPL_SERIALIZE}"
}
)";

int main(int argc, char* argv[]) {
	auto transport = Transport(init_doc);
	auto callback = [](const string& sending_topic,
	                   const string& listening_topic, const Message& message) {
		cout << "subscriber callback with"
		     << " from=" << sending_topic << " to=" << listening_topic
		     << " payload=" << message.payload
		     << " attributes=" << message.attributes << endl;
	};
	auto subscriber = Subscriber(transport, "upl/*", callback, "Zenoh");
	sleep(10);
}
