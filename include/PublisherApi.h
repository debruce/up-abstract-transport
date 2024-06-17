#pragma once

#include "TransportBase.h"

namespace UpAbstractTransport {
struct PublisherApi {
	typedef std::shared_ptr<PublisherApi> (*Getter)(Transport,
	                                                const std::string&);
	virtual void operator()(const Message&) = 0;
};

class Publisher {
	std::shared_ptr<PublisherApi> pImpl;

public:
	Publisher(Transport, const std::string&,
	          const TransportTag& tag = "default");
	void operator()(const Message& message) { (*pImpl)(message); }
};
};  // namespace UpAbstractTransport