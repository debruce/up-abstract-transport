#include "UpAbstractTransport.hpp"
#include "FactoryPlugin.hpp"
#include <iostream>

using namespace UpAbstractTransport;
using namespace std;

namespace Impl_serializer
{
    struct SerialImpl : public UpAbstractTransport::SerialApi
    {
        SerialImpl()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~SerialImpl()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        std::any get_factory(const std::string &name)
        {
            cout << __PRETTY_FUNCTION__ << ' ' << name << endl;
            return "hello";
        }

        static shared_ptr<UpAbstractTransport::SerialApi> get_instance()
        {
            return make_shared<SerialImpl>();
        }
    };
}; // Impl_serializer

UpAbstractTransport::SerializerFactories factories = {
    Impl_serializer::SerialImpl::get_instance};
FACTORY_EXPOSE(factories);
