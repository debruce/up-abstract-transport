#pragma once

#include "UpAbstractTransport.hpp"
#include "FactoryPlugin.hpp"

namespace UpAbstractTransport
{
    struct HiddenTransport {
        FactoryPlugin<ConceptFactories> conceptPlugin;
        std::shared_ptr<ConceptApi>    conceptImpl;
        FactoryPlugin<SerializerFactories> serialPlugin;
        std::shared_ptr<SerialApi>   serialImpl;

        HiddenTransport(const Doc &init_doc);
    };

}; // UpAbstractTransport