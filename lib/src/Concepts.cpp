#include <any>

#include "HiddenTransport.h"

namespace UpAbstractTransport {
using namespace std;

Publisher::Publisher(Transport transport, const string& topic) {
	auto a = transport.getConcept("publisher");
	auto getter = any_cast<PublisherApi::Getter>(a);
	pImpl = (*getter)(transport, topic);
}

Subscriber::Subscriber(Transport transport, const string& topic,
                       SubscriberCallback callback) {
	auto a = transport.getConcept("subscriber");
	auto getter = any_cast<SubscriberApi::Getter>(a);
	pImpl = (*getter)(transport, topic, callback);
}

RpcClient::RpcClient(Transport transport, const string& topic,
                     const Message& message,
                     const chrono::milliseconds& timeout) {
	auto a = transport.getConcept("rpc_client");
	auto getter = any_cast<RpcClientApi::Getter>(a);
	pImpl = (*getter)(transport, topic, message, timeout);
}

future<RpcReply> rpcCall(Transport transport, const string& topic,
                         const Message& message,
                         const chrono::milliseconds& timeout) {
	auto topicCopy = make_shared<string>(topic);
	auto msg = make_shared<Message>(message);
	return async(
	    [=]() { return RpcClient(transport, *topicCopy, *msg, timeout)(); });
}

RpcServer::RpcServer(Transport transport, const string& topic,
                     RpcServerCallback callback) {
	auto a = transport.getConcept("rpc_server");
	auto getter = any_cast<RpcServerApi::Getter>(a);
	pImpl = (*getter)(transport, topic, callback);
}

};  // namespace UpAbstractTransport