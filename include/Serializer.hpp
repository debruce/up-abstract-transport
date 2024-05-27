#pragma once

#include <memory>
#include <map>
#include <string>
#include <any>

namespace UpAbstractTransport
{
    using AnyMap = std::map<std::string, std::any>;

    struct SerializerApi
    {
        virtual std::string hello(const std::string &) = 0;
    };

    class Serializer
    {
        std::shared_ptr<SerializerApi> pImpl;

    public:
        std::string hello(const std::string &arg);

        friend class Transport;
    };
};
