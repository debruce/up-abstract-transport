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

void show(Transport transport, const string& name)
{
    auto obj = transport.getSerializer(name);
    auto am = obj.fetch(true);
    cout << "######## showing " << name << " = " << obj.messageName() << endl;
    anymapFormat(cout, am);
    cout << endl << endl;
}

int main(int argc, char* argv[])
{
    auto transport = Transport(init_doc);

    auto obj = transport.getSerializer("Outer");

    AnyMap am = {
        { "a", 1234 },
        { "b", 4321 },
        { "e", "X" },
        { "d", 0xdeadbeef }
    };
    anymapFormat(cout, am);
    // am["a"] = 1234;

    obj.assign(am);

    cout << "messageName = " << obj.messageName() << endl;
    cout << "debugString vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << endl;
    cout << obj.debugString();
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
    string serial_data = obj.serialize();
    cout << "serialized length = " << serial_data.size() << endl;
    cout << "data = ";
    for (auto c : serial_data) {
        cout << int(c) << ' ';
    }
    cout << endl << endl << endl << endl;

    auto obj2 = transport.getSerializer("Outer");
    obj2.deserialize(serial_data);
    cout << "debugString vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << endl;
    cout << obj2.debugString();
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
    auto ret2 = obj2.fetch();
    anymapFormat(cout, ret2);
}

