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
    {
        cout << "before load" << endl;
        auto transport = Transport(init_doc);
        cout << "after load" << endl;
    }
    cout << "after unload" << endl;
}

