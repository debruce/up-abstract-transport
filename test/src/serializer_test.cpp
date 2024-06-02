#include "UpAbstractTransport.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace UpAbstractTransport;

auto init_doc =
R"(
{
    "implementation": "${IMPL_ZENOH}",
    "serializers": "${IMPL_SERIALIZE}"
}
)";

void show(const Serializer& obj, bool describe = true)
{
    auto am = obj.fetch(describe);
    cout << "######## showing " << obj.messageName() << (describe ? " types" : " values") << endl;
    anymapFormat(cout, am);
    cout << endl << endl;
}

string to_hex(const string& s)
{
    stringstream ss;
    for (auto c : s) {
        ss << setfill('0') << setw(2) << hex << (int(c) & 0xff) << ' ';
    }
    return ss.str();
}

void test_uuri(Transport transport)
{
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$ testing uuri" << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    auto uuri = transport.getSerializer("UUri");
    show(uuri, true);
    AnyMap am {
        { "authority_name", "test_auth_name" },
        { "resource_id", 1234 },
        { "ue_id", 4321 },
        { "ue_version_major", 0xdeadbeef }
    };
    uuri.assign(am);
    show(uuri, false);

    auto packed = uuri.serialize();
    cout << "serialized ###########################################" << endl;
    cout << to_hex(packed) << endl;

    auto uuri2 = transport.getSerializer("UUri");
    uuri2.deserialize(packed);
    cout << "debugString ###########################################" << endl;
    cout << uuri2.debugString();  
}

void test_ustatus(Transport transport)
{
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$ testing ustatus" << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;

    auto ustatus = transport.getSerializer("UStatus");
    show(ustatus, true);
    AnyMap am {
        { "message", "test_msg_name" },
        { "code", "ALREADY_EXISTS" }
    };
    ustatus.assign(am);
    show(ustatus, false);

    auto packed = ustatus.serialize();
    cout << "serialized ###########################################" << endl;
    cout << to_hex(packed) << endl;

    auto ustatus2 = transport.getSerializer("UStatus");
    ustatus2.deserialize(packed);
    cout << "debugString ###########################################" << endl;
    cout << ustatus2.debugString();  
}

void test_uattributes(Transport transport)
{
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$ testing uattribues" << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;

    auto uattributes = transport.getSerializer("UAttributes");
    show(uattributes, true);
    AnyMap am {
        { "commstatus", "DEADLINE_EXCEEDED" },
        { "permission_level", 7 },
        { "token", "token_tester" },
        { "traceparent", "traceparent_tester"},
        { "ttl", 8 },
        { "id", AnyMap{ {"lsb", 5}, {"msb", 6} } },
        { "payload_format", "UPAYLOAD_FORMAT_PROTOBUF_WRAPPED_IN_ANY" },
        { "priority", "UPRIORITY_CS1" },
        { "reqid", AnyMap{ {"lsb", 10}, {"msb", 11} } },
        { "sink", AnyMap{ { "authority_name", "aname1" }, { "resource_id", 30 }, { "ue_id", 31 }, { "ue_version_major", 32 } } },
        { "source", AnyMap{ { "authority_name", "aname2" }, { "resource_id", 40 }, { "ue_id", 41 }, { "ue_version_major", 42 } } },
        { "type", "UMESSAGE_TYPE_RESPONSE" }
    };
    uattributes.assign(am);
    show(uattributes, false);

    auto packed = uattributes.serialize();
    cout << "serialized ###########################################" << endl;
    cout << to_hex(packed) << endl;

    auto uattributes2 = transport.getSerializer("UAttributes");
    uattributes2.deserialize(packed);
    cout << "debugString ###########################################" << endl;
    cout << uattributes2.debugString();  
}

int main(int argc, char* argv[])
{
    auto transport = Transport(init_doc);

    test_uuri(transport);

    test_ustatus(transport);

    test_uattributes(transport);  
}

