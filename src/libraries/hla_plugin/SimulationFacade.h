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

#include "tools/MessageObserver.h"

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

namespace plugins
{
namespace hla
{
    class UnitTypeResolver_ABC;
    class RemoteAgentSubject_ABC;
    class ContextFactory_ABC;
    template< typename Message > class ContextHandler_ABC;
    class AutomatDisengager;
    class FormationCreater;
    class AutomatCreater;
    class UnitTeleporter;
    class RemoteAgentController;

// =============================================================================
/** @class  SimulationFacade
    @brief  Simulation facade
*/
// Created: SLI 2011-09-15
// =============================================================================
class SimulationFacade : private tools::MessageObserver< sword::ControlEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             SimulationFacade( tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                               dispatcher::SimulationPublisher_ABC& publisher, dispatcher::Model_ABC& dynamicModel,
                               const dispatcher::StaticModel& staticModel, const UnitTypeResolver_ABC& unitTypeResolver,
                               RemoteAgentSubject_ABC& remoteAgentSubject );
    virtual ~SimulationFacade();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify( const sword::ControlEndTick& message, int context );
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageController_ABC< sword::SimToClient_Content >& messageController_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    dispatcher::Model_ABC& dynamicModel_;
    const dispatcher::StaticModel& staticModel_;
    const UnitTypeResolver_ABC& unitTypeResolver_;
    RemoteAgentSubject_ABC& remoteAgentSubject_;
    std::auto_ptr< ContextFactory_ABC > pContextFactory_;
    std::auto_ptr< ContextHandler_ABC< sword::FormationCreation > > pFormationHandler_;
    std::auto_ptr< ContextHandler_ABC< sword::AutomatCreation > > pAutomatHandler_;
    std::auto_ptr< ContextHandler_ABC< sword::UnitCreation > > pUnitHandler_;
    std::auto_ptr< AutomatDisengager > pAutomatDisengager_;
    std::auto_ptr< FormationCreater > pFormationCreater_;
    std::auto_ptr< AutomatCreater > pAutomatCreater_;
    std::auto_ptr< UnitTeleporter > pUnitTeleporter_;
    std::auto_ptr< RemoteAgentController > pRemoteAgentController_;
    //@}
};

}
}

#endif // plugins_hla_SimulationFacade_h
