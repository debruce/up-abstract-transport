#include "UpAbstractTransport.hpp"
#include "Impl_zenoh.hpp"
#include <iostream>

namespace Impl_zenoh {

using namespace UpAbstractTransport;
using namespace std;

struct RpcServerImpl : public RpcServerApi {
    RpcServerImpl(Transport transport, const string& topic, RpcServerCallback callback)
    {
        cout << __PRETTY_FUNCTION__ << " topic=" << topic << endl; 
    }
};

std::shared_ptr<RpcServerApi> rpc_server_getter(Transport transport, const string& topic, RpcServerCallback callback)
{
    return make_shared<RpcServerImpl>(transport, topic, callback);
}

}; // Impl_zenoh