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
}

namespace sword
{
    class ControlEndTick;
    class FormationCreation;
    class AutomatCreation;
    class UnitCreation;
    class SimToClient_Content;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Model_ABC;
    class StaticModel;
}

namespace rpr
{
    class EntityTypeResolver_ABC;
}

namespace plugins
{
namespace hla
{
    class UnitTypeResolver_ABC;
    class RemoteAgentSubject_ABC;
    class ContextFactory_ABC;
    class ComponentTypes_ABC;
    class AutomatDisengager;
    class FormationCreater;
    class AutomatCreater;
    class UnitTeleporter;
    class EquipmentUpdater;
    class RemoteAgentController;

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
             SimulationFacade( const ContextFactory_ABC& contextFactory, tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                               dispatcher::SimulationPublisher_ABC& publisher, dispatcher::Model_ABC& dynamicModel, const rpr::EntityTypeResolver_ABC& componentTypeResolver,
                               const dispatcher::StaticModel& staticModel, const UnitTypeResolver_ABC& unitTypeResolver,
                               RemoteAgentSubject_ABC& remoteAgentSubject, const ComponentTypes_ABC& componentTypes );
    virtual ~SimulationFacade();
    //@}

    //! @name Operations
    //@{
    virtual void Register( ResponseObserver_ABC< sword::UnitCreation >& observer );
    virtual void Unregister( ResponseObserver_ABC< sword::UnitCreation >& observer );
    virtual void Send( simulation::UnitMagicAction& message, const std::string& identifier );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ContextHandler_ABC< sword::FormationCreation > > pFormationHandler_;
    std::auto_ptr< ContextHandler_ABC< sword::AutomatCreation > > pAutomatHandler_;
    std::auto_ptr< ContextHandler_ABC< sword::UnitCreation > > pUnitHandler_;
    std::auto_ptr< AutomatDisengager > pAutomatDisengager_;
    std::auto_ptr< FormationCreater > pFormationCreater_;
    std::auto_ptr< AutomatCreater > pAutomatCreater_;
    std::auto_ptr< UnitTeleporter > pUnitTeleporter_;
    std::auto_ptr< EquipmentUpdater > pEquipmentUpdater_;
    std::auto_ptr< RemoteAgentController > pRemoteAgentController_;
    //@}
};

}
}

#endif // plugins_hla_SimulationFacade_h
