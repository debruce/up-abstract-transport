#include "HiddenTransport.h"
#include "TransportImpl.h"
#include "Utils.h"

using namespace UpAbstractTransport;
using namespace std;

struct SInfoImpl {
	string source;
	Message message;

	SInfoImpl(const zenohc::Sample& sample) {
		source = sample.get_keyexpr().as_string_view();
		message.payload = sample.get_payload().as_string_view();
		message.attributes =
		    sample.get_attachment().get("attributes").as_string_view();
	}
};

struct SubscriberImpl : public SubscriberApi {
	shared_ptr<TransportImpl> trans_impl;
	unique_ptr<zenohc::Subscriber> handle;
	string listening_topic;
	Fifo<SInfoImpl> fifo;
	unique_ptr<ThreadPool> pool;
	zenohc::KeyExprView expr;
	SubscriberCallback callback;

	void handler(const zenohc::Sample& sample) {
		fifo.push(make_shared<SInfoImpl>(sample));
	}

	void worker() {
		using namespace std;

		while (true) {
			auto ptr = fifo.pull();
			if (ptr == nullptr)
				break;
			callback(ptr->source, listening_topic, ptr->message);
		}
	}

	SubscriberImpl(Transport transport, const std::string& topic,
	               SubscriberCallback _callback, const TransportTag& tag)
	    : expr(topic) {
		trans_impl = transport.pImpl->getTransportImpl<TransportImpl>("Zenoh");
		listening_topic = topic;
		callback = _callback;
		handle =
		    make_unique<zenohc::Subscriber>(zenohc::expect<zenohc::Subscriber>(
		        trans_impl->session.declare_subscriber(
		            expr,
		            [&](const zenohc::Sample& arg) { this->handler(arg); })));
		pool = make_unique<ThreadPool>([&]() { worker(); }, 4);
	}

	~SubscriberImpl() { fifo.exit(); }
};

shared_ptr<SubscriberApi> subscriber_getter(Transport transport,
                                            const std::string& topic,
                                            SubscriberCallback callback,
                                            const TransportTag& tag) {
	return make_shared<SubscriberImpl>(transport, topic, callback, tag);
}
