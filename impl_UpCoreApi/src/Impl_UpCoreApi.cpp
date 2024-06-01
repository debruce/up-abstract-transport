#include "HiddenTransport.hpp"
#include "FactoryPlugin.hpp"
#include "uprotocol/v1/uattributes.pb.h"
#include "ProtobufSerializerApi.hpp"
#include <sstream>

using namespace UpAbstractTransport;
using namespace std;

string removeNewlines(const string& arg)
{
    string ret;
    ret.reserve(arg.size());
    for (auto c : arg) {
        if (c == '\n') ret += ' ';
        else ret += c;
    }
    return ret;
}
namespace Impl_UpCoreApi
{
    struct Impl : public UpAbstractTransport::ProtobufSerializerApi
    {
        uprotocol::v1::UAttributes* uattr;

        Impl(const string &kind)
        {
            if (kind == "UAttributes")
            {
                uattr = new uprotocol::v1::UAttributes();
                msg_ptr = uattr;
            }
            else if (kind == "UStatus")
            {
                uattr = nullptr;
                msg_ptr = new uprotocol::v1::UStatus();
            }
            else if (kind == "UUri")
            {
                uattr = nullptr;
                msg_ptr = new uprotocol::v1::UUri();
            }
            else
            {
                stringstream ss;
                ss << "UpCoreApi plugin does not implement \"" << kind << "\"";
                throw runtime_error(ss.str());
            }
        }

        std::string validate() const override
        {
            if (uattr) {
                cout << "############### validate uattributes ###############" << endl;
                // cout << "uuri = " << uattr->id().lsb() << ' ' << uattr->id().msb() << endl;
                // cout << "type = " << uattr->type() << endl;
                // cout << "priority = " << uattr->priority() << endl;
                cout << removeNewlines(uattr->DebugString()) << endl;
            }
            return string();
        }

        static shared_ptr<UpAbstractTransport::SerializerApi> getInstance(const string &kind)
        {
            return make_shared<Impl>(kind);
        }
    };
}; // Impl_UpCoreApi

UpAbstractTransport::SerializerFactories factories = {
    Impl_UpCoreApi::Impl::getInstance};

FACTORY_EXPOSE(factories);
