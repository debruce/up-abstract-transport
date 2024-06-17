#pragma once

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

struct Concept {
	FactoryPlugin<ConceptFactories> plugin;
	std::shared_ptr<ConceptApi> impl;
};


struct HiddenTransport {
	// FactoryPlugin<ConceptFactories> conceptPlugin;
	// std::shared_ptr<ConceptApi> conceptImpl;
	FactoryPlugin<SerializerFactories> serialPlugin;
	Concept concept;

	HiddenTransport(const Doc& init_doc);
};

};  // namespace UpAbstractTransport