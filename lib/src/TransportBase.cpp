#include <wordexp.h>

#include <any>
#include <filesystem>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

#include "HiddenTransport.h"

namespace UpAbstractTransport {
using namespace std;

string resolve_path(const string& impl) {
	wordexp_t we;
	auto retval = wordexp(impl.c_str(), &we, WRDE_UNDEF);
	if (retval != 0) {
		stringstream ss;
		ss << "wordexp expansion failed on \"" << impl << '"';
		throw runtime_error(ss.str());
	}
	if (we.we_wordc != 1) {
		wordfree(&we);
		stringstream ss;
		ss << "wordexp expanded to more than one path on \"" << impl << '"';
		throw runtime_error(ss.str());
	}
	string result(we.we_wordv[0]);
	wordfree(&we);
	return result;
}

HiddenTransport::HiddenTransport(const Doc& init_doc) {
	string path;

	path = resolve_path(init_doc["serializers"].get<string>());
	serialPlugin = FactoryPlugin<SerializerFactories>(path);

	path = resolve_path(init_doc["Zenoh"].get<string>());
	auto plugin = FactoryPlugin<ConceptFactories>(path);
	transports.emplace("Zenoh", TransportPlugin{plugin, plugin->getImplementation(init_doc)} );
}

Transport::Transport(const Doc& init_doc)
    : pImpl(new HiddenTransport(init_doc)) {}

Transport::Transport(const char* init_string)
    : Transport(Doc::parse(init_string)) {}

Serializer Transport::getSerializer(const string& name) {
	Serializer ret;

	ret.pImpl = pImpl->serialPlugin->get_instance(name);
	return ret;
}

any Transport::getConcept(const TransportTag& tag, const string& name) {
	auto it = pImpl->transports.find(tag.name);
	if (it == pImpl->transports.end()) {
		throw runtime_error("Cannot find transport from tag");
	}
	return it->second.impl->getConcept(name);
}

// vector<string> Transport::listConcepts() {
// 	return pImpl->conceptImpl->listConcepts();
// }

};  // namespace UpAbstractTransport