#include "HiddenTransport.h"
#include "TransportImpl.h"
#include "Utils.h"
#include "uprotocol/v1/umessage.pb.h"
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

using namespace UpAbstractTransport;
using namespace std;

struct SInfoImpl {
	sockaddr_in addr;
	int	fd;

	SInfoImpl(int fd, const sockaddr_in& addr) : fd(fd), addr(addr) {}

	~SInfoImpl()
	{
		if (fd >= 0) close(fd);
	}
};

struct SubscriberImpl : public SubscriberApi {
	shared_ptr<TransportImpl> trans_impl;
	int socket_fd;
	thread tid;
	atomic_bool die;
	string listening_topic;
	Fifo<SInfoImpl> fifo;
	unique_ptr<ThreadPool> pool;
	SubscriberCallback callback;

	void worker()
	{
		while (true) {
			auto ptr = fifo.pull();
			if (ptr == nullptr)
				break;
			array<char, 32768>	buffer;
			int len;
			while ((len = recv(ptr->fd, buffer.data(), buffer.size(), 0)) > 0) {
				auto msg = new uprotocol::v1::UMessage();
				msg->ParseFromArray(buffer.data(), len);
				UpAbstractTransport::Message message;
				message.payload = msg->payload();
				message.attributes = msg->attributes().SerializeAsString();
				callback("socket_source", listening_topic, message);
				// cout << "'''" << endl << msg->DebugString() << "'''" << endl;
				// stringstream ss;
				// ss << "[ ";
				// for (auto i = 0; i < len; i++) {
				// 	ss << hex << setw(2) << buffer[i] << ' ';
				// }
				// ss << ']';
				// cout << ss.str() << endl;
			}
			close(ptr->fd);
		}
	}

	void acceptWorker() {
		while (!die) {
			sockaddr_in addr;
			socklen_t addrLen = sizeof(addr);
			auto data_fd = accept(socket_fd, (sockaddr*)&addr, &addrLen);
			if (data_fd < 0) {
				cerr << "subscriber accept data_fd=" << data_fd << endl;
			}
			else {
				cout << "accepted " << data_fd << endl;
				fifo.push(make_shared<SInfoImpl>(data_fd, addr));
			}
		}
	}

	SubscriberImpl(Transport transport, const std::string& topic,
	               SubscriberCallback _callback, const TransportTag& tag)
	    : listening_topic(topic) {
		trans_impl = transport.pImpl->getTransportImpl<TransportImpl>("UdpSocket");
		cout << __PRETTY_FUNCTION__ << endl;
		callback = _callback;

		const char* ipv4 = tag.get<const char*>("ipv4");
		short port = tag.get<int>("port");

		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd < 0) {
			cerr << "socket failed" << endl;
			exit(-1);
		}

		struct sockaddr_in addr = { AF_INET, htons(port) };
		addr.sin_addr.s_addr = inet_addr(ipv4);
		if (bind(socket_fd, (const sockaddr*)&addr, sizeof(addr)) < 0) {
			cerr << "bind failed" << endl;
			exit(-1);
		}

		listen(socket_fd, 5);
		die = false;
		tid = thread([&]() { acceptWorker(); });
		pool = make_unique<ThreadPool>([&]() { worker(); }, 4);
	}

	~SubscriberImpl()
	{
		die = true;
		tid.join();
		if (socket_fd != -1) {
			close(socket_fd);
			socket_fd = -1;
		}
		fifo.exit();
	}
};

shared_ptr<SubscriberApi> subscriber_getter(Transport transport,
                                            const std::string& topic,
                                            SubscriberCallback callback,
                                            const TransportTag& tag) {
	return make_shared<SubscriberImpl>(transport, topic, callback, tag);
}
