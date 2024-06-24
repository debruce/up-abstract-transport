#include "HiddenTransport.h"
#include "TransportImpl.h"
#include "uprotocol/v1/umessage.pb.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

using namespace UpAbstractTransport;
using namespace std;

struct RpcClientImpl : public RpcClientApi {
	shared_ptr<TransportImpl> trans_impl;
	int socket_fd;

	RpcClientImpl(Transport transport, const string& topic,
	              const Message& message, const chrono::milliseconds& timeout,
	              const TransportTag& tag) {
		cout << __PRETTY_FUNCTION__ << endl;

		const char* ipv4 = tag.get<const char*>("ipv4");
		short port = tag.get<int>("port");

		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd < 0) {
			cerr << "socket failed" << endl;
			exit(-1);
		}

		struct sockaddr_in addr = { AF_INET, htons(port) };
		addr.sin_addr.s_addr = inet_addr(ipv4);
		if (connect(socket_fd, (const sockaddr*)&addr, sizeof(addr)) < 0) {
			cerr << "connect failed" << endl;
			exit(-1);
		}
		cout << "after connect" << endl;
		//
		// Below ugliness to be replaced with protobuf-free merging function
		//
		auto attr = new uprotocol::v1::UAttributes();
		attr->ParseFromString(message.attributes);
		auto pmsg = new uprotocol::v1::UMessage();
		pmsg->set_allocated_attributes(attr);
		pmsg->set_payload(message.payload);
		auto data = pmsg->SerializeAsString();
		//
		// Above ugliness to be replaced with protobuf-free merging function
		//
		if (send(socket_fd, data.data(), data.size(), 0) < 0) {
			cerr << "send failed" << endl;
			exit(-1);
		}
		cout << "after send" << endl;
	}

	~RpcClientImpl()
	{
		if (socket_fd != -1) {
			close(socket_fd);
			socket_fd = -1;
		}
	}

	RpcReply operator()() override {
		array<uint8_t, 32768>	buffer;
		auto len = recv(socket_fd, buffer.data(), buffer.size(), 0);
		cout << "recv got len=" << len << endl;
		if (len < 0) {
			cerr << "recv failed" << endl;
			exit(-1);
		}
		//
		// Below ugliness to be replaced with protobuf-free splitting function
		//
		auto msg = new uprotocol::v1::UMessage();
		msg->ParseFromArray(buffer.data(), buffer.size());
		cout << msg->DebugString() << endl;
		// string src;
		Message message;
		message.payload = msg->payload();
		message.attributes = msg->attributes().SerializeAsString();
		//
		// Above ugliness to be replaced with protobuf-free splitting function
		//
		return message;
	}
};

std::shared_ptr<RpcClientApi> rpc_client_getter(
    Transport transport, const std::string& topic, const Message& message,
    const chrono::milliseconds& timeout, const TransportTag& tag) {
	return make_shared<RpcClientImpl>(transport, topic, message, timeout, tag);
}
