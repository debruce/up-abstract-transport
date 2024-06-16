#pragma once

#include <memory>
#include <map>
#include <string>
#include <any>
#include <functional>

namespace UpAbstractTransport
{
    using AnyMap = std::map<std::string, std::any>;

    void anymapFormat(std::ostream& os, const AnyMap& m, const std::string& pad = "    ", size_t depth=0, bool with_type=false);

    struct SerializerApi
    {
        virtual std::string messageName() const = 0;
        virtual std::string debugString() const = 0;
        virtual std::string serialize() const = 0;
        virtual bool deserialize(const std::string&) = 0;
        virtual bool assign(const AnyMap& arg) = 0;
        virtual AnyMap fetch(bool describe) const = 0;
    };

    class Serializer
    {
        std::shared_ptr<SerializerApi> pImpl;

    public:
        std::string messageName() const;
        std::string debugString() const;
        std::string serialize() const;
        bool deserialize(const std::string&);
        bool assign(const AnyMap& arg);
        AnyMap fetch(bool describe = false) const;

        friend class Transport;
    };
};
