#pragma once

#include <memory>
#include <map>
#include <string>
#include <any>

namespace UpAbstractTransport
{
    using AnyMap = std::map<std::string, std::any>;

    void anymapFormat(std::ostream& os, const AnyMap& m, const std::string& pad = "    ", size_t depth=0, bool with_type=false);

    struct SerializerApi
    {
        virtual std::string messageName() = 0;
        virtual std::string debugString() = 0;
        virtual std::string serialize() = 0;
        virtual bool deserialize(const std::string&) = 0;
        virtual bool assign(const AnyMap& arg) = 0;
        virtual AnyMap fetch(bool describe) = 0;
    };

    class Serializer
    {
        std::shared_ptr<SerializerApi> pImpl;

    public:
        std::string messageName();
        std::string debugString();
        std::string serialize();
        bool deserialize(const std::string&);
        bool assign(const AnyMap& arg);
        AnyMap fetch(bool describe = false);

        friend class Transport;
    };
};
