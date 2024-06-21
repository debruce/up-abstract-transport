#pragma once

#include <sys/sdt.h>

#include <map>
#include <nlohmann/json.hpp>
#include <string>

#include "HiddenTransport.h"
#include "Trace.h"
#include "zenohc.hxx"

struct TransportImpl : public UpAbstractTransport::ConceptApi,
                       public TraceBase {
	zenohc::Session session;
	std::map<std::string, std::any> getters;

	TransportImpl(const nlohmann::json& doc);
	~TransportImpl();

	std::any getConcept(const std::string&) override;
	UpAbstractTransport::Doc describe() const override;

	static std::shared_ptr<UpAbstractTransport::ConceptApi> getImplementation(
	    const nlohmann::json& doc);
};
