#include "HiddenTransport.hpp"
#include "Impl_zenoh.hpp"
#include "FactoryPlugin.hpp"
#include <iostream>
#include <sstream>

namespace Impl_zenoh
{

    using namespace UpAbstractTransport;
    using namespace std;

    shared_ptr<PublisherApi> publisher_getter(Transport transport, const string &name);
    shared_ptr<SubscriberApi> subscriber_getter(Transport transport, const string &topic, SubscriberCallback callback);
    shared_ptr<RpcClientApi> rpc_client_getter(Transport transport, const string &topic, const Message &message, const chrono::milliseconds &timeout);
    shared_ptr<RpcServerApi> rpc_server_getter(Transport transport, const string &topic, RpcServerCallback callback);

    static zenohc::Session inst()
    {
        zenohc::Config config;
        return zenohc::expect<zenohc::Session>(zenohc::open(std::move(config)));
    }

    TransportImpl::TransportImpl(const nlohmann::json &doc) : TraceBase("TrImpl"), session(inst())
    {
        TRACE(this, "");
        getters["publisher"] = publisher_getter;
        getters["subscriber"] = subscriber_getter;
        getters["rpc_client"] = rpc_client_getter;
        getters["rpc_server"] = rpc_server_getter;
    }

    TransportImpl::~TransportImpl()
    {
        TRACE(this, "");
    }

    any TransportImpl::getConcept(const std::string &name)
    {
        auto it = getters.find(name);
        if (it == getters.end()) {
            stringstream ss;
            ss << "Cannot find getter for concept \"" << name << '"';
            throw runtime_error(ss.str()); 
        }
        return it->second;
    }

    vector<string> TransportImpl::listConcepts()
    {
        vector<string> ret;
        ret.reserve(getters.size());
        for (const auto& [k, v] : getters) ret.push_back(k);
        return ret;
    }


    std::shared_ptr<UpAbstractTransport::ConceptApi> TransportImpl::getImplementation(const nlohmann::json &doc)
    {
        return make_shared<TransportImpl>(doc);
    }

}; // Impl_zenoh

UpAbstractTransport::ConceptFactories factories = {
    Impl_zenoh::TransportImpl::getImplementation};
FACTORY_EXPOSE(factories);