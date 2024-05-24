#include "UpAbstractTransport.hpp"
#include "Impl_zenoh.hpp"
#include <iostream>

namespace Impl_zenoh {

using namespace UpAbstractTransport;
using namespace std;

struct SubscriberImpl : public SubscriberApi {
    SubscriberImpl(Transport transport, const std::string& topic, SubscriberServerCallback)
    {
        cout << __PRETTY_FUNCTION__ << " topic=" << topic << endl; 
    }
};

std::shared_ptr<SubscriberApi> subscriber_getter(Transport transport, const std::string& topic, SubscriberServerCallback callback)
{
    return make_shared<SubscriberImpl>(transport, topic, callback);
}

}; // Impl_zenoh