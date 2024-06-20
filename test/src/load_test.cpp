#include <unistd.h>

#include <iostream>

#include "UpAbstractTransport.h"

using namespace std;
using namespace UpAbstractTransport;

int main(int argc, char* argv[]) {
	{
		cout << "before load" << endl;
		auto transport = Transport();
		cout << "after load" << endl;
		// auto concepts = transport.listConcepts();
		// for (const auto k : concepts) {
		// 	cout << "    concept: " << k << endl;
		// }
	}
	cout << "after unload" << endl;
}
