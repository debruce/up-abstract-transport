#pragma once

#include "zenohc.hxx"
#include "Trace.hpp"
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <sys/sdt.h>

namespace Impl_zenoh
{
    struct TransportImpl : public UpAbstractTransport::ConceptApi, public TraceBase
    {
        zenohc::Session session;
        std::map<std::string, std::any> getters;

        TransportImpl(const nlohmann::json &doc);
        ~TransportImpl();

        std::any getConcept(const std::string &) override;
        std::vector<std::string> listConcepts() override;

        static std::shared_ptr<UpAbstractTransport::ConceptApi> getImplementation(const nlohmann::json &doc);
    };

}; // Impl_zenoh