#include <unistd.h>

#include <iostream>

#include "UpAbstractTransport.h"

using namespace std;
using namespace UpAbstractTransport;

auto init_doc =
    R"(
{
    "implementation": "${IMPL_ZENOH}",
    "serializers": "${IMPL_SERIALIZE}"
}
)";

int main(int argc, char* argv[]) {
	{
		cout << "before load" << endl;
		auto transport = Transport(init_doc);
		cout << "after load" << endl;
		auto concepts = transport.listConcepts();
		for (const auto k : concepts) {
			cout << "    concept: " << k << endl;
		}
	}
	cout << "after unload" << endl;
}
