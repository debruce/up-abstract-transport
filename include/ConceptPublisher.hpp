#pragma once

#include "TransportBase.hpp"

namespace UpAbstractTransport
{
    struct PublisherApi
    {
        typedef std::shared_ptr<PublisherApi> (*Getter)(Transport, const std::string &);
        virtual void operator()(const Message &) = 0;
    };

    class Publisher
    {
        std::shared_ptr<PublisherApi> pImpl;

    public:
        Publisher(Transport, const std::string &);
        void operator()(const Message &message) { (*pImpl)(message); }
    };
};