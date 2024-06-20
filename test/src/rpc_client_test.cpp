#include <unistd.h>

#include <iostream>

#include "UpAbstractTransport.h"

using namespace std;
using namespace UpAbstractTransport;

template <class... Args>
string genString(const char* fmt, Args... args) {
	char buf[128];
	snprintf(buf, sizeof(buf), fmt, args...);
	return string(buf);
}

int main(int argc, char* argv[]) {
	auto transport = Transport();

	for (auto i = 0; i < 5; i++) {
		auto f = rpcCallFuture(
		    transport, "demo/rpc/action1",
		    Message{genString("pay_A_%d", i), genString("attr_A_%d", i)},
		    chrono::seconds(2), "Zenoh");
		auto result = f.get();
		cout << "got result payload=" << result->payload
		     << " attributes=" << result->attributes << endl;
		usleep(100000);
	}
}
