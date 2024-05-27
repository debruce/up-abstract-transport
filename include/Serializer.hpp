#pragma once

#include "TransportBase.hpp"

namespace UpAbstractTransport
{
    struct SerializerApi
    {
        virtual std::string hello(const std::string &) = 0;
    };

    class Serializer
    {
        std::shared_ptr<SerializerApi> pImpl;

    public:
        Serializer(Transport, const std::string &);
        std::string hello(const std::string &arg);
    };
};
