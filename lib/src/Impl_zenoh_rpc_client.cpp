#include "UpAbstractTransport.hpp"
#include "Impl_zenoh.hpp"
#include <iostream>

namespace Impl_zenoh {

using namespace UpAbstractTransport;
using namespace std;

struct RpcClientImpl : public RpcClientApi {
    RpcClientImpl(Transport transport, const std::string& topic, const Message& message, const std::chrono::seconds& timeout)
    {
        cout << __PRETTY_FUNCTION__ << " topic=" << topic << endl; 
    }

    tuple<string, Message> operator()() override
    {
        cout << __PRETTY_FUNCTION__ << endl;
        return make_tuple(string("hello"), Message{});      
    }
};

std::shared_ptr<RpcClientApi> rpc_client_getter(Transport transport, const std::string& topic, const Message& message, const std::chrono::seconds& timeout)
{
    return make_shared<RpcClientImpl>(transport, topic, message, timeout);
}

}; // Impl_zenoh