#include "UpAbstractTransport.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace UpAbstractTransport;

int main(int argc, char* argv[])
{
    auto init_doc =
R"(
{
    "transport": "zenoh"
}
)";
    auto transport = Transport(init_doc);
    auto callback = [](const string& sending_topic, const string& listening_topic, const Message& message) -> RpcReply {
        cout << "rpc callback with"
            << " from=" << sending_topic
            << " to=" << listening_topic
            << " payload=" << message.payload
            << " attributes=" << message.attributes << endl;
        return Message{"from", "server"};
    };
    auto rpc_server = RpcServer(transport, "demo/rpc/action1", callback);
    sleep(10000);
}
