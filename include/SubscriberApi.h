#pragma once

#include "TransportBase.h"

namespace UpAbstractTransport {
using SubscriberCallback =
    std::function<void(const std::string& sending_topic,
                       const std::string& listening_topic, const Message&)>;

struct SubscriberApi {
	typedef std::shared_ptr<SubscriberApi> (*Getter)(Transport,
	                                                 const std::string&,
	                                                 SubscriberCallback);
};

class Subscriber {
	std::shared_ptr<SubscriberApi> pImpl;

public:
	Subscriber(Transport, const std::string&, SubscriberCallback);
};
};  // namespace UpAbstractTransport