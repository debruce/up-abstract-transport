#pragma once

#include "TransportBase.h"

namespace UpAbstractTransport {
using RpcServerCallback =
    std::function<RpcReply(const std::string& sending_topic,
                           const std::string& listening_topic, const Message&)>;

struct RpcServerApi {
	typedef std::shared_ptr<RpcServerApi> (*Getter)(Transport,
	                                                const std::string&,
	                                                RpcServerCallback);
};

class RpcServer {
	std::shared_ptr<RpcServerApi> pImpl;

public:
	RpcServer(Transport, const std::string&, RpcServerCallback);
};
};  // namespace UpAbstractTransport