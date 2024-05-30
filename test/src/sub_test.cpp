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

int main(int argc, char *argv[])
{
    auto transport = Transport(init_doc);
    auto callback = [](const string &sending_topic, const string &listening_topic, const Message &message)
    {
        cout << "subscriber callback with"
             << " from=" << sending_topic
             << " to=" << listening_topic
             << " payload=" << message.payload
             << " attributes=" << message.attributes << endl;
    };
    auto subscriber = Subscriber(transport, "upl/*", callback);
    sleep(10);
}
