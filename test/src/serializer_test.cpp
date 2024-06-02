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
    cout << "######## showing " << obj.messageName() << endl;
    anymapFormat(cout, am);
    cout << endl << endl;
}

string to_hex(const string& s)
{
    stringstream ss;
    for (auto c : s) {
        ss << setfill('0') << setw(2) << hex << int(c) << ' ';
    }
    return ss.str();
}

void test_uuri(Transport transport)
{
    auto uuri = transport.getSerializer("UUri");
    show(uuri, true);
    AnyMap am {
        { "authority_name", "test_auth_name" },
        { "resource_id", 1234 },
        { "ue_id", 4321 },
        { "ue_version", 0xdeadbeef }
    };
    uuri.assign(am);
    show(uuri, false);
}

int main(int argc, char* argv[])
{
    auto transport = Transport(init_doc);

    test_uuri(transport);


    // auto ustatus = transport.getSerializer("UStatus");
    // show(ustatus);

    // auto attr = transport.getSerializer("UAttributes");
    // show(attr);

    // AnyMap am;
    // am["commstatus"] = "DEADLINE_EXCEEDED";
    // am["permission_level"] = 7;
    // am["token"] = "hello";
    // am["traceparent"] = "goodbye";
    // am["ttl"] = 8;
    // am["id"] = AnyMap{ {"lsb", 5}, {"msb", 6} };
    // am["payload_format"] = "UPAYLOAD_FORMAT_PROTOBUF_WRAPPED_IN_ANY";
    // am["priority"] = "UPRIORITY_CS1";
    // am["reqid"] = AnyMap{ {"lsb", 10}, {"msb", 11} };
    // am["sink"] = AnyMap{ { "authority_name", "aname1" }, { "resource_id", 30 }, { "ue_id", 31 }, { "ue_version_major", 32 } };
    // am["source"] = AnyMap{ { "authority_name", "aname2" }, { "resource_id", 40 }, { "ue_id", 41 }, { "ue_version_major", 42 } };
    // am["type"] = "UMESSAGE_TYPE_RESPONSE";
    // attr.assign(am);
    // cout << "debugString ###########################################" << endl;
    // cout << attr.debugString();

    // auto packed = attr.serialize();
    // cout << "serialized ###########################################" << endl;
    // cout << to_hex(packed) << endl;

    // auto attr2 = transport.getSerializer("UAttributes");
    // attr2.deserialize(packed);
    // cout << "debugString ###########################################" << endl;
    // cout << attr.debugString();    
}

