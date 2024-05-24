#include "UpAbstractTransport.hpp"
#include "Impl_zenoh.hpp"

namespace Impl_zenoh {

using namespace UpAbstractTransport;
using namespace std;

static zenohc::Session inst()
{
    zenohc::Config config;
    return zenohc::expect<zenohc::Session>(zenohc::open(std::move(config)));
}

TransportImpl::TransportImpl(const nlohmann::json& doc) : session(inst())
{
}

TransportImpl::~TransportImpl()
{
}

any transport_getter(const nlohmann::json& doc)
{
    return make_shared<TransportImpl>(doc);
}

}; // Impl_zenoh