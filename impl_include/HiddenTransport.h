#pragma once

#include <map>

#include "FactoryPlugin.h"
#include "UpAbstractTransport.h"

namespace UpAbstractTransport {
struct ConceptApi {
	virtual std::any getConcept(const std::string&) = 0;
	virtual Doc describe() const = 0;
};

struct ConceptFactories {
	std::function<std::shared_ptr<ConceptApi>(const Doc& init_doc)>
	    getImplementation;
};

struct SerializerFactories {
	std::function<std::shared_ptr<SerializerApi>(const std::string& kind)>
	    getInstance;
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

	Serializer getSerializer(const std::string& name) {
		Serializer ret;
		ret.pImpl = serialPlugin->getInstance(name);
		return ret;
	}

	std::any getConcept(const std::string& tagName, const std::string& conceptName) {
		auto it = transports.find(tagName);
		if (it == transports.end()) {
			throw std::runtime_error("Cannot find transport from tag");
		}
		return it->second.impl->getConcept(conceptName);
	}

	Doc describe() const
	{
		using namespace std;
		Doc	ret;
		ret["serializers"]["path"] = serialPlugin.getPath();
		ret["serializers"]["MD5"] = serialPlugin.getMD5();
		for (const auto& [k, v] : transports) {
			ret[k]["path"] = v.plugin.getPath();
			ret[k]["MD5"] = v.plugin.getMD5();
			ret[k]["concepts"] = v.impl->describe();
		}
		return ret;
	}

};

};  // namespace UpAbstractTransport