// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_HlaPlugin_h
#define plugins_hla_HlaPlugin_h

#include "dispatcher/Plugin_ABC.h"
#include "protocol/Protocol.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Config;
    class Model_ABC;
    class Logger_ABC;
    class SimulationPublisher_ABC;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace plugins
{
namespace hla
{
    class FederateFacade;
    class RtiAmbassadorFactory_ABC;
    class FederateAmbassadorFactory_ABC;
    class AgentSubject_ABC;

// =============================================================================
/** @class  HlaPlugin
    @brief  Hla plugin
*/
// Created: SBO 2008-02-18
// =============================================================================
class HlaPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             HlaPlugin( dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Config& config, xml::xistream& xis, dispatcher::Logger_ABC& logger );
    virtual ~HlaPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );

    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Member data
    //@{
    const bool timeConstrained_;
    dispatcher::Model_ABC& model_;
    dispatcher::Logger_ABC& logger_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    std::auto_ptr< RtiAmbassadorFactory_ABC > pRtiFactory_;
    std::auto_ptr< RtiAmbassadorFactory_ABC > pDebugRtiFactory_;
    std::auto_ptr< FederateAmbassadorFactory_ABC > pFederateFactory_;
    std::auto_ptr< FederateAmbassadorFactory_ABC > pDebugFederateFactory_;
    std::auto_ptr< rpr::EntityTypeResolver_ABC > pResolver_;
    std::auto_ptr< AgentSubject_ABC > pSubject_;
    std::auto_ptr< FederateFacade > federate_;
    //@}
};

}
}

#endif // plugins_hla_HlaPlugin_h
