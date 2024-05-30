#include "UpAbstractTransport.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace UpAbstractTransport;

#if 0
auto init_doc =
R"(
{
    "implementation": "${IMPL_ZENOH}",
    "serializers": "${IMPL_SERIALIZE}"
}
)";
#else
auto init_doc =
R"(
{
    "implementation": "${IMPL_ZENOH}"
}
)";
#endif

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

    for (auto i = 0; i < 5; i++) {
        auto f = rpcCall(transport, "demo/rpc/action1", Message{genString("pay_A_%d", i), genString("attr_A_%d", i)}, chrono::seconds(2));
        auto result = f.get();
        cout << "got result payload=" << result->payload << " attributes=" << result->attributes <<  endl;
        usleep(100000);
    }
}

