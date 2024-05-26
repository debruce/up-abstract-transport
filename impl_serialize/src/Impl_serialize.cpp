#include "UpAbstractTransport.hpp"
#include "FactoryPlugin.hpp"
#include <iostream>

#if 0
namespace Impl_serial
{

    using namespace UpAbstractTransport;
    using namespace std;

    // shared_ptr<PublisherApi> publisher_getter(Transport transport, const string &name);
    // shared_ptr<SubscriberApi> subscriber_getter(Transport transport, const string &topic, SubscriberCallback callback);
    // shared_ptr<RpcClientApi> rpc_client_getter(Transport transport, const string &topic, const Message &message, const chrono::milliseconds &timeout);
    // shared_ptr<RpcServerApi> rpc_server_getter(Transport transport, const string &topic, RpcServerCallback callback);

    SerialImpl::SerialImpl(const nlohmann::json &doc)
    {
        // getters["publisher"] = publisher_getter;
        // getters["subscriber"] = subscriber_getter;
        // getters["rpc_client"] = rpc_client_getter;
        // getters["rpc_server"] = rpc_server_getter;
    }

    SerialImpl::~SerialImpl()
    {
    }

    any SerialImpl::get_factory(const std::string &name)
    {
        return getters[name];
    }

    std::shared_ptr<UpAbstractTransport::SerialApi> SerialImpl::get_instance()
    {
        return make_shared<SerialImpl>();
    }

}; // Impl_serial

UpAbstractTransport::SerialFactories factories = {
    Impl_serial::SerialImpl::get_instance};
FACTORY_EXPOSE(factories);
#endif