#pragma once

#include "zenohc.hxx"
#include <nlohmann/json.hpp>

namespace Impl_zenoh {

struct TransportImpl {
    zenohc::Session session;

    TransportImpl(const nlohmann::json& doc);
    ~TransportImpl();
};

}; // Impl_zenoh