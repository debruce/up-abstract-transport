#pragma once

#include <map>

#include "FactoryPlugin.h"
#include "UpAbstractTransport.h"

namespace UpAbstractTransport {
struct ConceptApi {
	virtual std::any getConcept(const std::string&) = 0;
	virtual std::vector<std::string> listConcepts() = 0;
};

struct ConceptFactories {
	std::function<std::shared_ptr<ConceptApi>(const Doc& init_doc)>
	    getImplementation;
};

struct SerializerFactories {
	std::function<std::shared_ptr<SerializerApi>(const std::string& kind)>
	    get_instance;
};

struct TransportPlugin {
	FactoryPlugin<ConceptFactories> plugin;
	std::shared_ptr<ConceptApi> impl;
};

struct HiddenTransport {
	FactoryPlugin<SerializerFactories> serialPlugin;
	std::map<std::string, TransportPlugin> transports;

	HiddenTransport(const Doc& init_doc);

	template <typename T>
	std::shared_ptr<T> getTransportImpl(const std::string& kind) {
		auto it = transports.find(kind);
		if (it == transports.end()) {
			throw std::runtime_error(
			    std::string("getTransportImpl annot find ") + kind);
		}
		return std::dynamic_pointer_cast<T>(it->second.impl);
	}
};

};  // namespace UpAbstractTransport