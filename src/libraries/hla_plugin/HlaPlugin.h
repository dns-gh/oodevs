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

namespace kernel
{
    class CoordinateConverter_ABC;
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

namespace tic
{
    class PlatformDelegateFactory_ABC;
}

namespace sword
{
    class SimToClient_Content;
    class MessengerToClient_Content;
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
    class DotationTypeResolver_ABC;
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
    class NETN_InteractionBuilder_ABC;
    class InteractionBuilder;
    class SimulationFacade;
    class DetonationFacade;
    class TransportationFacade;
    class TacticalObjectController;
    class OwnershipStrategy_ABC;
    class OwnershipController;
    class OwnershipPolicy_ABC;
    class TransferSender_ABC;
    class EntityIdentifierResolver_ABC;
    class FOM_Serializer_ABC;
    class SideResolver_ABC;
    class SimulationTimeManager_ABC;
    class MRMController;

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
    virtual void Receive( const sword::MessengerToClient& message );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& dynamicModel_;
    const dispatcher::StaticModel& staticModel_;
    dispatcher::SimulationPublisher_ABC& simulationPublisher_;
    dispatcher::ClientPublisher_ABC& clientsPublisher_;
    const dispatcher::Config& config_;
    std::unique_ptr< xml::xistream > pXisSession_;
    std::unique_ptr< xml::xistream > pXisConfiguration_;
    std::unique_ptr< xml::ximultistream > pXis_;
    std::unique_ptr< dispatcher::Logger_ABC > logFilter_;
    dispatcher::Logger_ABC& logger_;
    std::unique_ptr< ContextFactory_ABC > pContextFactory_;
    std::unique_ptr< ObjectResolver_ABC > pObjectResolver_;
    std::unique_ptr< RtiAmbassadorFactory_ABC > pRtiFactory_;
    std::unique_ptr< RtiAmbassadorFactory_ABC > pDebugRtiFactory_;
    std::unique_ptr< FederateAmbassadorFactory_ABC > pFederateFactory_;
    std::unique_ptr< FederateAmbassadorFactory_ABC > pDebugFederateFactory_;
    std::unique_ptr< rpr::EntityTypeResolver_ABC > pAggregateTypeResolver_;
    std::unique_ptr< rpr::EntityTypeResolver_ABC > pAutomatAggregateResolver_;
    std::unique_ptr< rpr::EntityTypeResolver_ABC > pSurfaceVesselTypeResolver_;
    std::unique_ptr< rpr::EntityTypeResolver_ABC > pComponentTypeResolver_;
    std::unique_ptr< rpr::EntityTypeResolver_ABC > pEntityMunitionTypeResolver_;
    std::unique_ptr< rpr::EntityTypeResolver_ABC > pEntityObjectTypeResolver_;
    std::unique_ptr< ComponentTypes_ABC > pComponentTypes_;
    std::unique_ptr< UnitTypeResolver_ABC > pUnitTypeResolver_;
    std::unique_ptr< UnitTypeResolver_ABC > pAutomatTypeResolver_;
    std::unique_ptr< DotationTypeResolver_ABC > pMunitionTypeResolver_;
    std::unique_ptr< LocalAgentResolver_ABC > pLocalAgentResolver_;
    std::unique_ptr< CallsignResolver_ABC > pCallsignResolver_;
    std::unique_ptr< MissionResolver_ABC > pMissionResolver_;
    std::unique_ptr< Subordinates_ABC > pSubordinates_;
    std::unique_ptr< tools::MessageController< sword::SimToClient_Content > > pMessageController_;
    std::unique_ptr< AutomatChecker_ABC > pAutomatChecker_;
    std::unique_ptr< kernel::CoordinateConverter_ABC > pConverter_;
    std::unique_ptr< ExtentResolver_ABC > pExtentResolver_;
    std::unique_ptr< SideResolver_ABC > pSideResolver_;
    std::unique_ptr< AgentController > pSubject_;
    std::unique_ptr< TacticalObjectController > pTacticalObjectSubject_;
    std::unique_ptr< FOM_Serializer_ABC > pFomSerializer_;
    std::unique_ptr< FederateFacade > pFederate_;
    std::unique_ptr< NETN_InteractionBuilder_ABC > pNetnInteractionBuilder_;
    std::unique_ptr< InteractionBuilder > pInteractionBuilder_;
    std::unique_ptr< SimulationFacade > pSimulationFacade_;
    std::unique_ptr< RemoteAgentResolver_ABC > pRemoteAgentResolver_;
    std::unique_ptr< DetonationFacade > pDetonationFacade_;
    std::unique_ptr< SideChecker_ABC > pSideChecker_;
    std::unique_ptr< TransportationFacade > pTransportationFacade_;
    std::unique_ptr< Stepper > pStepper_;
    std::unique_ptr< tic::PlatformDelegateFactory_ABC > platforms_;
    std::unique_ptr< tools::MessageController< sword::MessengerToClient_Content > > pMessengerMessageController_;
    std::unique_ptr< OwnershipStrategy_ABC > pOwnershipStrategy_;
    std::unique_ptr< TransferSender_ABC > transferSender_;
    std::unique_ptr< OwnershipController > pOwnershipController_;
    std::unique_ptr< OwnershipPolicy_ABC > pOwnershipPolicy_;
    std::unique_ptr< EntityIdentifierResolver_ABC > pEntityIdentifierResolver_;
    std::unique_ptr< SimulationTimeManager_ABC > pSimulationTimeManager_;
    std::unique_ptr< MRMController > pMrmController;
    //@}
};

}
}

#endif // plugins_hla_HlaPlugin_h
