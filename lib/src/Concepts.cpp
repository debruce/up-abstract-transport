#include <any>
#include <iostream>

#include "HiddenTransport.h"

namespace UpAbstractTransport {
using namespace std;

Publisher::Publisher(Transport transport, const string& topic,
                     const TransportTag& tag) {
	auto a = transport.getConcept(tag, "publisher");
	auto getter = any_cast<PublisherApi::Getter>(a);
	pImpl = (*getter)(transport, topic, tag);
}

Subscriber::Subscriber(Transport transport, const string& topic,
                       SubscriberCallback callback, const TransportTag& tag) {
	auto a = transport.getConcept(tag, "subscriber");
	auto getter = any_cast<SubscriberApi::Getter>(a);
	pImpl = (*getter)(transport, topic, callback, tag);
}

RpcClientFuture::RpcClientFuture(Transport transport, const string& topic,
                                 const Message& message,
                                 const chrono::milliseconds& timeout,
                                 const TransportTag& tag) {
	auto a = transport.getConcept(tag, "rpc_client");
	auto getter = any_cast<RpcClientApi::Getter>(a);
	pImpl = (*getter)(transport, topic, message, timeout, tag);
}

future<RpcReply> rpcCallFuture(Transport transport, const string& topic,
                               const Message& message,
                               const chrono::milliseconds& timeout,
                               const TransportTag& tag) {
	auto topicCopy = make_shared<string>(topic);
	auto msg = make_shared<Message>(message);
	auto tagCopy = make_shared<TransportTag>(tag);
	return async([=]() {
		return RpcClientFuture(transport, *topicCopy, *msg, timeout,
		                       *tagCopy)();
	});
}

RpcServer::RpcServer(Transport transport, const string& topic,
                     RpcServerCallback callback, const TransportTag& tag) {
	auto a = transport.getConcept(tag, "rpc_server");
	auto getter = any_cast<RpcServerApi::Getter>(a);
	pImpl = (*getter)(transport, topic, callback, tag);
}

};  // namespace UpAbstractTransport