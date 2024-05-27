#pragma once

#include "TransportBase.hpp"

namespace UpAbstractTransport
{
    struct RpcClientApi
    {
        typedef std::shared_ptr<RpcClientApi> (*Getter)(Transport, const std::string &, const Message &, const std::chrono::milliseconds &);
        virtual RpcReply operator()() = 0;
    };

    class RpcClient
    {
        std::shared_ptr<RpcClientApi> pImpl;

    public:
        RpcClient(Transport, const std::string &, const Message &, const std::chrono::milliseconds &);

        RpcReply operator()() { return (*pImpl)(); }
    };

    std::future<RpcReply> rpcCall(Transport, const std::string &, const Message &, const std::chrono::milliseconds &);
};