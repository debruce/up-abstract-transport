#include "HiddenTransport.h"
#include "TransportImpl.h"

using namespace UpAbstractTransport;
using namespace std;

template <typename T>
static z_bytes_t make_zbytes(const T& t) {
	return {.len = t.size(), .start = (const uint8_t*)t.data()};
}

struct PublisherImpl : public PublisherApi {
	shared_ptr<TransportImpl> trans_impl;
	z_owned_publisher_t handle;

	PublisherImpl(Transport transport, const std::string& expr) {
		trans_impl =
		    dynamic_pointer_cast<TransportImpl>(transport.pImpl->transports["Zenoh"].impl);
		handle = z_declare_publisher(trans_impl->session.loan(),
		                             z_keyexpr(expr.c_str()), nullptr);
		if (!z_check(handle))
			throw std::runtime_error("Cannot declare publisher");
	}

	~PublisherImpl() { z_undeclare_publisher(&handle); }

	void operator()(const Message& msg) override {
		z_publisher_put_options_t options = z_publisher_put_options_default();
		z_owned_bytes_map_t map = z_bytes_map_new();
		options.attachment = z_bytes_map_as_attachment(&map);
		z_bytes_map_insert_by_alias(&map, z_bytes_new("attributes"),
		                            make_zbytes(msg.attributes));
		if (z_publisher_put(z_loan(handle), (const uint8_t*)msg.payload.data(),
		                    msg.payload.size(), &options)) {
			z_drop(z_move(map));
			throw std::runtime_error("Cannot publish");
		}
		z_drop(z_move(map));
	}
};

std::shared_ptr<PublisherApi> publisher_getter(Transport transport,
                                               const std::string& name) {
	return make_shared<PublisherImpl>(transport, name);
}
