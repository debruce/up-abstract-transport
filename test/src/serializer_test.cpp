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

int main(int argc, char* argv[])
{
    auto transport = Transport(init_doc);
    auto obj = transport.get_serializer("hello");
    auto ret = obj.hello("arg to hello");
    cout << "ret = " << ret  << endl;
}

