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

struct RInfoImpl {
	sockaddr_in addr;
	int	fd;

	RInfoImpl(int fd, const sockaddr_in& addr)  : fd(fd), addr(addr) {}

	~RInfoImpl()
	{
		if (fd >= 0) close(fd);
	}
};

struct RpcServerImpl : public RpcServerApi {
	shared_ptr<TransportImpl> trans_impl;
	int socket_fd;
	thread tid;
	atomic_bool die;
	string listening_topic;
	Fifo<RInfoImpl> fifo;
	unique_ptr<ThreadPool> pool;
	RpcServerCallback callback;

	void worker()
	{
		while (true) {
			auto ptr = fifo.pull();
			if (ptr == nullptr)
				break;
			array<char, 32768>	buffer;
			int len;
			while ((len = recv(ptr->fd, buffer.data(), buffer.size(), 0)) > 0) {
				//
				// Below ugliness to be replaced with protobuf-free splitting function
				//
				auto msg = new uprotocol::v1::UMessage();
				msg->ParseFromArray(buffer.data(), len);
				UpAbstractTransport::Message message;
				message.payload = msg->payload();
				message.attributes = msg->attributes().SerializeAsString();
				//
				// Above ugliness to be replaced with protobuf-free splitting function
				//

				auto response = callback("socket_source", listening_topic, message);
				if (response) {
					//
					// Below ugliness to be replaced with protobuf-free merging function
					//
					auto attr = new uprotocol::v1::UAttributes();
					attr->ParseFromString(response->attributes);
					auto respMsg = new uprotocol::v1::UMessage();
					respMsg->set_payload(response->payload);
					respMsg->set_allocated_attributes(attr);
					auto outgoing = respMsg->SerializeAsString();
					//
					// Above ugliness to be replaced with protobuf-free merging function
					//
					if (send(ptr->fd, outgoing.data(), outgoing.size(), 0) < 0) {
						cout << "rpc server send failed" << endl;
					}
				}
			}
			close(ptr->fd);
		}
	}

	void acceptWorker() {
		cout << "top of acceptWorker" << endl;
		while (!die) {
			sockaddr_in addr;
			socklen_t addrLen = sizeof(addr);
			auto data_fd = accept(socket_fd, (sockaddr*)&addr, &addrLen);
			cout << "acceptWorker got fd=" << data_fd << endl;
			if (data_fd < 0) {
				cerr << "rpc server accept data_fd=" << data_fd << endl;
			}
			else {
				cout << "rpc server accepted " << data_fd << endl;
				fifo.push(make_shared<RInfoImpl>(data_fd, addr));
			}
		}
	}

	RpcServerImpl(Transport transport, const string& topic,
	              RpcServerCallback _callback, const TransportTag& tag)
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
		cout << "socket_fd = " << socket_fd << endl;
		die = false;
		tid = thread([&]() { acceptWorker(); });
		pool = make_unique<ThreadPool>([&]() { worker(); }, 4);
	}

	~RpcServerImpl() {
		cout << __PRETTY_FUNCTION__ << endl;
		die = true;
		tid.join();
		if (socket_fd != -1) {
			close(socket_fd);
			socket_fd = -1;
		}
		fifo.exit();
	}
};

std::shared_ptr<RpcServerApi> rpc_server_getter(Transport transport,
                                                const string& topic,
                                                RpcServerCallback callback,
                                                const TransportTag& tag) {
	return make_shared<RpcServerImpl>(transport, topic, callback, tag);
}
