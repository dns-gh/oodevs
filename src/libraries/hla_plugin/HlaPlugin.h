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
#include <memory>

namespace xml
{
    class xistream;
    class ximultistream;
}

namespace dispatcher
{
    class Config;
    class Model_ABC;
    class Logger_ABC;
    class SimulationPublisher_ABC;
    class ClientPublisher_ABC;
    class StaticModel;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace sword
{
    class SimToClient_Content;
}

namespace tools
{
    template< typename category > class MessageController;
}

namespace plugins
{
namespace hla
{
    class FederateFacade;
    class RtiAmbassadorFactory_ABC;
    class FederateAmbassadorFactory_ABC;
    class AgentController;
    class ObjectResolver_ABC;
    class Stepper;
    class UnitTypeResolver_ABC;
    class MunitionTypeResolver_ABC;
    class LocalAgentResolver_ABC;
    class RemoteAgentResolver_ABC;
    class CallsignResolver_ABC;
    class SideChecker_ABC;
    class ComponentTypes_ABC;
    class ContextFactory_ABC;
    class MissionResolver_ABC;
    class AutomatChecker_ABC;
    class Subordinates_ABC;
    class ExtentResolver_ABC;
    class InteractionBuilder;
    class SimulationFacade;
    class DetonationFacade;
    class TransportationFacade;

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
             HlaPlugin( dispatcher::Model_ABC& dynamicModel, const dispatcher::StaticModel& staticModel,
                        dispatcher::SimulationPublisher_ABC& simulationPublisher, dispatcher::ClientPublisher_ABC& clientsPublisher,
                        const dispatcher::Config& config, xml::xistream& xis, dispatcher::Logger_ABC& logger );
    virtual ~HlaPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& dynamicModel_;
    const dispatcher::StaticModel& staticModel_;
    dispatcher::SimulationPublisher_ABC& simulationPublisher_;
    dispatcher::ClientPublisher_ABC& clientsPublisher_;
    const dispatcher::Config& config_;
    std::auto_ptr< xml::xistream > pXisSession_;
    std::auto_ptr< xml::xistream > pXisConfiguration_;
    std::auto_ptr< xml::ximultistream > pXis_;
    std::auto_ptr< dispatcher::Logger_ABC > logFilter_;
    dispatcher::Logger_ABC& logger_;
    std::auto_ptr< ContextFactory_ABC > pContextFactory_;
    std::auto_ptr< ObjectResolver_ABC > pObjectResolver_;
    std::auto_ptr< RtiAmbassadorFactory_ABC > pRtiFactory_;
    std::auto_ptr< RtiAmbassadorFactory_ABC > pDebugRtiFactory_;
    std::auto_ptr< FederateAmbassadorFactory_ABC > pFederateFactory_;
    std::auto_ptr< FederateAmbassadorFactory_ABC > pDebugFederateFactory_;
    std::auto_ptr< rpr::EntityTypeResolver_ABC > pAggregateTypeResolver_;
    std::auto_ptr< rpr::EntityTypeResolver_ABC > pSurfaceVesselTypeResolver_;
    std::auto_ptr< rpr::EntityTypeResolver_ABC > pComponentTypeResolver_;
    std::auto_ptr< rpr::EntityTypeResolver_ABC > pEntityMunitionTypeResolver_;
    std::auto_ptr< ComponentTypes_ABC > pComponentTypes_;
    std::auto_ptr< UnitTypeResolver_ABC > pUnitTypeResolver_;
    std::auto_ptr< MunitionTypeResolver_ABC > pMunitionTypeResolver_;
    std::auto_ptr< LocalAgentResolver_ABC > pLocalAgentResolver_;
    std::auto_ptr< CallsignResolver_ABC > pCallsignResolver_;
    std::auto_ptr< MissionResolver_ABC > pMissionResolver_;
    std::auto_ptr< Subordinates_ABC > pSubordinates_;
    std::auto_ptr< tools::MessageController< sword::SimToClient_Content > > pMessageController_;
    std::auto_ptr< AutomatChecker_ABC > pAutomatChecker_;
    std::auto_ptr< ExtentResolver_ABC > pExtentResolver_;
    std::auto_ptr< AgentController > pSubject_;
    std::auto_ptr< FederateFacade > pFederate_;
    std::auto_ptr< InteractionBuilder > pInteractionBuilder_;
    std::auto_ptr< SimulationFacade > pSimulationFacade_;
    std::auto_ptr< RemoteAgentResolver_ABC > pRemoteAgentResolver_;
    std::auto_ptr< DetonationFacade > pDetonationFacade_;
    std::auto_ptr< SideChecker_ABC > pSideChecker_;
    std::auto_ptr< TransportationFacade > pTransportationFacade_;
    std::auto_ptr< Stepper > pStepper_;
    //@}
};

}
}

#endif // plugins_hla_HlaPlugin_h
