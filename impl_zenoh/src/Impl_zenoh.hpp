#pragma once

#include "zenohc.hxx"
#include "Trace.hpp"
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <sys/sdt.h>

namespace Impl_zenoh {

struct TransportImpl : public UpAbstractTransport::TransportApi, public TraceBase {
    zenohc::Session session;
    std::map<std::string, std::any> getters;

    TransportImpl(const nlohmann::json& doc);
    ~TransportImpl();

    std::any get_factory(const std::string&);

    static std::shared_ptr<UpAbstractTransport::TransportApi> get_instance(const nlohmann::json& doc);
};

}; // Impl_zenoh