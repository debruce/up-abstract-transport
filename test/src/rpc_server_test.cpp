#include <unistd.h>

#include <algorithm>
#include <iostream>

#include "UpAbstractTransport.h"

using namespace std;
using namespace UpAbstractTransport;

int main(int argc, char* argv[]) {
	auto transport = Transport();
	auto callback = [](const string& sending_topic,
	                   const string& listening_topic,
	                   const Message& message) -> RpcReply {
		cout << "rpc callback with"
		     << " from=" << sending_topic << " to=" << listening_topic
		     << " payload=" << message.payload
		     << " attributes=" << message.attributes << endl;
		string upayload, uattributes;
		for (auto c : message.payload)
			upayload += std::toupper(c);
		for (auto c : message.attributes)
			uattributes += std::toupper(c);
		return Message{upayload, uattributes};
	};
	auto rpc_server = RpcServer(transport, "demo/rpc/*", callback, "Zenoh");
	sleep(10000);
}
