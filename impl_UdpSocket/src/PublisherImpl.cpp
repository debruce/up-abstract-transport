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

struct PublisherImpl : public PublisherApi {
	shared_ptr<TransportImpl> trans_impl;
	int socket_fd;

	PublisherImpl(Transport transport, const std::string& expr,
	              const TransportTag& tag) {
		trans_impl = transport.pImpl->getTransportImpl<TransportImpl>("UdpSocket");
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
	}

	~PublisherImpl()
	{
		if (socket_fd != -1) {
			close(socket_fd);
			socket_fd = -1;
		}
	}

	void operator()(const Message& msg) override {
		auto attr = new uprotocol::v1::UAttributes();
		attr->ParseFromString(msg.attributes);
		auto pmsg = new uprotocol::v1::UMessage();
		pmsg->set_allocated_attributes(attr);
		pmsg->set_payload(msg.payload);
		auto data = pmsg->SerializeAsString();
		if (send(socket_fd, data.data(), data.size(), 0) < 0) {
			cerr << "send failed" << endl;
			exit(-1);
		}

		// struct iovec iov[2];
		// iov[1].iov_base = const_cast<char*>(msg.attributes.data());
		// iov[1].iov_len = msg.attributes.size();
		// iov[0].iov_base = const_cast<char*>(msg.payload.data());
		// iov[0].iov_len = msg.payload.size();
		// struct msghdr msghdr = { nullptr, 0, iov, 2, nullptr, 0 };
		// if (sendmsg(socket_fd, &msghdr, 0) < 0) {
		// 	cerr << "sendmsg failed" << endl;
		// 	exit(-1);
		// }
	}
};

std::shared_ptr<PublisherApi> publisher_getter(Transport transport,
                                               const std::string& name,
                                               const TransportTag& tag) {
	return make_shared<PublisherImpl>(transport, name, tag);
}
