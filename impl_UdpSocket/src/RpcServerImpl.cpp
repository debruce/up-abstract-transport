#include "HiddenTransport.h"
#include "TransportImpl.h"
#include "Utils.h"
#include <iostream>

using namespace UpAbstractTransport;
using namespace std;

// static std::string keyexpr2string(const z_keyexpr_t& keyexpr) {
// 	z_owned_str_t keystr = z_keyexpr_to_string(keyexpr);
// 	std::string ret(z_loan(keystr));
// 	z_drop(z_move(keystr));
// 	return ret;
// }

// static string stringFromZBytes(const z_bytes_t& z) {
// 	return string((const char*)z.start, z.len);
// }

// static z_bytes_t zbytesFromOpaque(const string_view& od) {
// 	return z_bytes_t{.len = od.size(), .start = (uint8_t*)od.data()};
// }

// struct RpcRequestNode {
// 	string source;
// 	Message message;
// 	z_owned_query_t owned_query;

// 	RpcRequestNode(const z_query_t* query) {
// 		source = keyexpr2string(z_query_keyexpr(query));
// 		z_bytes_t pred = z_query_parameters(query);
// 		z_value_t value = z_query_value(query);
// 		message.payload = stringFromZBytes(value.payload);

// 		z_attachment_t attachment = z_query_attachment(query);
// 		if (!z_check(attachment))
// 			throw std::runtime_error("attachment is missing");
// 		z_bytes_t avalue =
// 		    z_attachment_get(attachment, z_bytes_new("attributes"));
// 		message.attributes = stringFromZBytes(avalue);
// 		owned_query = z_query_clone(query);
// 	}

// 	~RpcRequestNode() { z_query_drop(&owned_query); }

// 	void reply(const Message& message, const string& expr) {
// 		auto query = z_query_loan(&owned_query);
// 		z_query_reply_options_t options = z_query_reply_options_default();
// 		z_owned_bytes_map_t map = z_bytes_map_new();
// 		z_bytes_map_insert_by_alias(&map, z_bytes_new("attributes"),
// 		                            zbytesFromOpaque(message.attributes));
// 		options.attachment = z_bytes_map_as_attachment(&map);
// 		z_query_reply(&query, z_keyexpr(expr.c_str()),
// 		              (const uint8_t*)message.payload.data(),
// 		              message.payload.size(), &options);
// 	}
// };

struct RpcServerImpl : public RpcServerApi {
	shared_ptr<TransportImpl> trans_impl;
	// z_owned_queryable_t qable;
	string listening_topic;
	// zenohc::KeyExprView expr;
	// Fifo<RpcRequestNode> fifo;
	// unique_ptr<ThreadPool> pool;
	RpcServerCallback callback;

	// static void _handler(const z_query_t* query, void* context) {
	// 	auto me = reinterpret_cast<RpcServerImpl*>(context);
	// 	me->handler(query);
	// }

	// virtual void handler(const z_query_t* query) {
	// 	fifo.push(make_shared<RpcRequestNode>(query));
	// }

	// void worker() {
	// 	using namespace std;

	// 	while (true) {
	// 		auto ptr = fifo.pull();
	// 		if (ptr == nullptr)
	// 			break;
	// 		auto result = callback(ptr->source, listening_topic, ptr->message);
	// 		if (result) {
	// 			ptr->reply(*result, listening_topic);
	// 		}
	// 	}
	// }

	RpcServerImpl(Transport transport, const string& topic,
	              RpcServerCallback _callback, const TransportTag& tag)
	    : listening_topic(topic) {
		trans_impl = transport.pImpl->getTransportImpl<TransportImpl>("UdpSocket");
		cout << __PRETTY_FUNCTION__ << endl;
		// listening_topic = topic;
		callback = _callback;
		// z_owned_closure_query_t closure = z_closure(_handler, NULL, this);
		// qable = z_declare_queryable(trans_impl->session.loan(), expr,
		//                             z_move(closure), NULL);
		// if (!z_check(qable))
		// 	throw std::runtime_error("Unable to create queryable.");
		// pool = make_unique<ThreadPool>([&]() { worker(); }, 4);
	}

	~RpcServerImpl() {
		// fifo.exit();
		// z_undeclare_queryable(z_move(qable));
	}
};

std::shared_ptr<RpcServerApi> rpc_server_getter(Transport transport,
                                                const string& topic,
                                                RpcServerCallback callback,
                                                const TransportTag& tag) {
	return make_shared<RpcServerImpl>(transport, topic, callback, tag);
}
