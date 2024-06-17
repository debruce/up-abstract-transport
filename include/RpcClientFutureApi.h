#pragma once

#include "TransportBase.h"

namespace UpAbstractTransport {
struct RpcClientApi {
	typedef std::shared_ptr<RpcClientApi> (*Getter)(
	    Transport, const std::string&, const Message&,
	    const std::chrono::milliseconds&, const TransportTag&);
	virtual RpcReply operator()() = 0;
};

class RpcClientFuture {
	std::shared_ptr<RpcClientApi> pImpl;

public:
	RpcClientFuture(Transport, const std::string&, const Message&,
	                const std::chrono::milliseconds&,
	                const TransportTag& = "Zenoh");

	RpcReply operator()() { return (*pImpl)(); }
};

std::future<RpcReply> rpcCallFuture(Transport, const std::string&,
                                    const Message&,
                                    const std::chrono::milliseconds&,
                                    const TransportTag& = "Zenoh");
};  // namespace UpAbstractTransport