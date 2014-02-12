// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_SimulationFacade_h
#define plugins_hla_SimulationFacade_h

#include "ContextHandler_ABC.h"

namespace tools
{
    template< typename Category > class MessageController_ABC;
    class ExerciseConfig;
}

namespace sword
{
    class ControlEndTick;
    class FormationCreation;
    class AutomatCreation;
    class UnitCreation;
    class SimToClient_Content;
    class ObjectMagicActionAck;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Model_ABC;
    class Logger_ABC;
    class StaticModel;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace xml
{
    class xisubstream;
}

namespace plugins
{
namespace hla
{
    class UnitTypeResolver_ABC;
    class RemoteAgentSubject_ABC;
    class ContextFactory_ABC;
    class ComponentTypes_ABC;
    class CallsignResolver_ABC;
    class AutomatTypeResolver_ABC;
    class ExtentResolver_ABC;
    class AutomatDisengager;
    class FormationCreater;
    class AutomatCreater;
    class UnitTeleporter;
    class EquipmentUpdater;
    class RemoteAgentController;
    class NetnRemoteCallsignListener;
    class AgentSubject_ABC;
    class LocalAgentResolver_ABC;
    class SideResolver_ABC;
    class RemoteTacticalObjectController;
    class RemoteTacticalObjectSubject_ABC;
    class RemoteOrbatShaper;
    class MissionResolver_ABC;
    class TacticalObjectUpdater;
    class PropagationManager_ABC;
    class SimulationTimeManager_ABC;
    

// =============================================================================
/** @class  SimulationFacade
    @brief  Simulation facade
*/
// Created: SLI 2011-09-15
// =============================================================================
class SimulationFacade : public ContextHandler_ABC< sword::UnitCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             SimulationFacade( xml::xisubstream xis, const ContextFactory_ABC& contextFactory, tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                               dispatcher::SimulationPublisher_ABC& publisher, dispatcher::Model_ABC& dynamicModel, const rpr::EntityTypeResolver_ABC& componentTypeResolver,
                               const UnitTypeResolver_ABC& unitTypeResolver, const UnitTypeResolver_ABC& automatTypeResolver,
                               RemoteAgentSubject_ABC& remoteAgentSubject, const ComponentTypes_ABC& componentTypes, CallsignResolver_ABC& callsignResolver,
                               dispatcher::Logger_ABC& logger, const ExtentResolver_ABC& extent, AgentSubject_ABC& subject, const LocalAgentResolver_ABC& localResolver,
                               const SideResolver_ABC& sideResolver, const rpr::EntityTypeResolver_ABC& objectEntityTypeResolver, RemoteTacticalObjectSubject_ABC& remoteTacticalSubject,
                               const MissionResolver_ABC& missionResolver, const tools::ExerciseConfig& config, const SimulationTimeManager_ABC& timeManager );
    virtual ~SimulationFacade();
    //@}

    //! @name Operations
    //@{
    virtual void Register( ResponseObserver_ABC< sword::UnitCreation >& observer );
    virtual void Unregister( ResponseObserver_ABC< sword::UnitCreation >& observer );
    virtual void Send( simulation::UnitMagicAction& message, const std::string& identifier );
    virtual void Send( simulation::ObjectMagicAction& message, const std::string& identifier );
    //@}

private:
    //! @name Member data
    //@{
    const SideResolver_ABC& sideResolver_;
    std::auto_ptr< ContextHandler_ABC< sword::FormationCreation > > pFormationHandler_;
    std::auto_ptr< ContextHandler_ABC< sword::AutomatCreation > > pAutomatHandler_;
    std::auto_ptr< ContextHandler_ABC< sword::UnitCreation > > pUnitHandler_;
    std::auto_ptr< ContextHandler_ABC< sword::ObjectMagicActionAck > > pObjectHandler_;
    std::auto_ptr< AutomatDisengager > pAutomatDisengager_;
    std::auto_ptr< FormationCreater > pFormationCreater_;
    std::auto_ptr< AutomatCreater > pAutomatCreater_;
    std::auto_ptr< UnitTeleporter > pUnitTeleporter_;
    std::auto_ptr< EquipmentUpdater > pEquipmentUpdater_;
    std::auto_ptr< RemoteAgentController > pRemoteAgentController_;
    std::auto_ptr< NetnRemoteCallsignListener > pNetnRemoteCallsignListener_;
    std::auto_ptr< PropagationManager_ABC > pPropagationManager_;
    std::auto_ptr< RemoteTacticalObjectController > pRemoteTacticalObjectController_;
    std::auto_ptr< RemoteOrbatShaper > pRemoteOrbatShaper_;
    std::auto_ptr< TacticalObjectUpdater > pTacticalObjectUpdater_;
    //@}
};

}
}

#endif // plugins_hla_SimulationFacade_h
