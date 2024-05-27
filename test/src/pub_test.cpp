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

template <class... Args>
string genString(const char* fmt, Args... args)
{
    char buf[128];
    snprintf(buf, sizeof(buf), fmt, args...);
    return string(buf);
}

int main(int argc, char* argv[])
{
    auto transport = Transport(init_doc);

    auto p1 = Publisher(transport, "upl/p1");
    auto p2 = Publisher(transport, "upl/p2");

    for (auto i = 0; i < 5; i++) {
        cout << endl << "client code pubishing " << i << endl;
        p1(Message{genString("pay_A_%d", i), genString("attr_A_%d", i)});
        p2(Message{genString("pay_B_%d", i), genString("attr_B_%d", i)});
        usleep(100000);
    }
}

