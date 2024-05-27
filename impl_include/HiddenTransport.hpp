#pragma once

#include "UpAbstractTransport.hpp"
#include "FactoryPlugin.hpp"

namespace UpAbstractTransport
{
    struct ConceptApi
    {
        virtual std::any getConcept(const std::string &) = 0;
        virtual std::vector<std::string> listConcepts() = 0;
    };
    
    struct ConceptFactories
    {
        std::function<std::shared_ptr<ConceptApi>(const Doc &init_doc)> getImplementation;
    };

    struct SerializerFactories
    {
        std::function<std::shared_ptr<SerializerApi>(const std::string& kind)> get_instance;
    };

    struct HiddenTransport {
        FactoryPlugin<ConceptFactories> conceptPlugin;
        std::shared_ptr<ConceptApi>    conceptImpl;
        FactoryPlugin<SerializerFactories> serialPlugin;

        HiddenTransport(const Doc &init_doc);
    };

}; // UpAbstractTransport