#include "HiddenTransport.hpp"
#include "FactoryPlugin.hpp"
#include "uprotocol/v1/uattributes.pb.h"
#include "AnyMap.hpp"
#include <iostream>

using namespace UpAbstractTransport;
using namespace std;
namespace gpb = google::protobuf;

namespace Impl_serializer
{
    struct Impl : public UpAbstractTransport::SerializerApi
    {
        string name;
        gpb::Message* msg_ptr;

        Impl(const string& kind)
        {
            name = kind;
            if (kind == "UAttributes") {
                msg_ptr = new uprotocol::v1::UAttributes();
            }
            else if (kind == "UStatus") {
                msg_ptr = new uprotocol::v1::UStatus();
            }
            else throw runtime_error("Message kind is not supported.");
        }

        ~Impl()
        {
            delete msg_ptr;
        }

        string messageName() const override
        {
            return msg_ptr->GetTypeName();
        }

        string debugString() const override
        {
            return msg_ptr->DebugString();
        }

        string serialize() const override
        {
            return msg_ptr->SerializeAsString();
        }

        bool deserialize(const string& arg) override
        {
            return msg_ptr->ParseFromString(arg);
        }

        bool assign(const AnyMap& arg) override
        {
            Anymap2Protobuf s(arg, *msg_ptr);
            return s.is_valid();
        }

        AnyMap fetch(bool describe) const override
        {
            return protobuf2anymap(*msg_ptr, describe);
        }

        static shared_ptr<UpAbstractTransport::SerializerApi> getInstance(const string& kind)
        {
            return make_shared<Impl>(kind);
        }
    };
}; // Impl_serializer

UpAbstractTransport::SerializerFactories factories = {
    Impl_serializer::Impl::getInstance};
FACTORY_EXPOSE(factories);
