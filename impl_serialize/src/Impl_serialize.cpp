#include "UpAbstractTransport.hpp"
#include "FactoryPlugin.hpp"
#include "proto_files/myprotocol.pb.h"
#include <iostream>

using namespace UpAbstractTransport;
using namespace std;

namespace Impl_serializer
{
    struct SerialObject : public UpAbstractTransport::SerializerApi
    {
        SerialObject(const string& kind)
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        ~SerialObject()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

        std::string hello(const std::string &name)
        {
            cout << __PRETTY_FUNCTION__ << ' ' << name << endl;
            return "hello";
        }

        static shared_ptr<UpAbstractTransport::SerializerApi> get_instance(const string& kind)
        {
            return make_shared<SerialObject>(kind);
        }
    };
}; // Impl_serializer

UpAbstractTransport::SerializerFactories factories = {
    Impl_serializer::SerialObject::get_instance};
FACTORY_EXPOSE(factories);
