#include <wordexp.h>

#include <any>
#include <filesystem>
#include <iostream>
#include <map>
#include <sstream>

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
	path = resolve_path(init_doc["Zenoh"].get<string>());
	conceptPlugin = FactoryPlugin<ConceptFactories>(path);
	conceptImpl = conceptPlugin->getImplementation(init_doc);

	if (init_doc.contains("serializers")) {
		path = resolve_path(init_doc["serializers"].get<string>());
		serialPlugin = FactoryPlugin<SerializerFactories>(path);
	}
}

Transport::Transport(const Doc& init_doc)
    : pImpl(new HiddenTransport(init_doc)) {}

Transport::Transport(const char* init_string)
    : Transport(Doc::parse(init_string)) {}

any Transport::getConcept(const string& name) {
	return pImpl->conceptImpl->getConcept(name);
}

vector<string> Transport::listConcepts() {
	return pImpl->conceptImpl->listConcepts();
}

Serializer Transport::getSerializer(const string& name) {
	Serializer ret;

	ret.pImpl = pImpl->serialPlugin->get_instance(name);
	return ret;
}

};  // namespace UpAbstractTransport