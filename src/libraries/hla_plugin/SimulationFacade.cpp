// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "SimulationFacade.h"
#include "ContextHandler.h"
#include "ContextFactory.h"
#include "AutomatDisengager.h"
#include "FormationCreater.h"
#include "AutomatCreater.h"
#include "RemoteAgentController.h"
#include "UnitTeleporter.h"
#include "protocol/Simulation.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/StaticModel.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/AgentTypes.h"

using namespace plugins::hla;

namespace
{
    class FormationContextHandler : public ContextHandler< sword::FormationCreation >
    {
    public:
        FormationContextHandler( tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                                 const ContextFactory_ABC& contextFactory, dispatcher::SimulationPublisher_ABC& publisher )
            : ContextHandler< sword::FormationCreation >( "formation", messageController, contextFactory, publisher )
        {
            CONNECT( messageController, *this, formation_creation );
        }
    };
    class AutomatContextHandler : public ContextHandler< sword::AutomatCreation >
    {
    public:
        AutomatContextHandler( tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                               const ContextFactory_ABC& contextFactory, dispatcher::SimulationPublisher_ABC& publisher )
            : ContextHandler< sword::AutomatCreation >( "automat", messageController, contextFactory, publisher )
        {
            CONNECT( messageController, *this, automat_creation );
        }
    };
    class UnitContextHandler : public ContextHandler< sword::UnitCreation >
    {
    public:
        UnitContextHandler( tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                            const ContextFactory_ABC& contextFactory, dispatcher::SimulationPublisher_ABC& publisher )
            : ContextHandler< sword::UnitCreation >( "unit", messageController, contextFactory, publisher )
        {
            CONNECT( messageController, *this, unit_creation );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: SimulationFacade constructor
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
SimulationFacade::SimulationFacade( tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                                    dispatcher::SimulationPublisher_ABC& publisher, dispatcher::Model_ABC& dynamicModel,
                                    const dispatcher::StaticModel& staticModel, const UnitTypeResolver_ABC& unitTypeResolver,
                                    RemoteAgentSubject_ABC& remoteAgentSubject, const LocalAgentResolver_ABC& localAgentResolver )
    : messageController_     ( messageController )
    , publisher_             ( publisher )
    , dynamicModel_          ( dynamicModel )
    , staticModel_           ( staticModel )
    , unitTypeResolver_      ( unitTypeResolver )
    , remoteAgentSubject_    ( remoteAgentSubject )
    , localAgentResolver_    ( localAgentResolver )
    , pContextFactory_       ( new ContextFactory() )
    , pFormationHandler_     ( new FormationContextHandler( messageController, *pContextFactory_, publisher ) )
    , pAutomatHandler_       ( new AutomatContextHandler( messageController, *pContextFactory_, publisher ) )
    , pUnitHandler_          ( new UnitContextHandler( messageController, *pContextFactory_, publisher ) )
    , pAutomatDisengager_    ( 0 )
    , pFormationCreater_     ( 0 )
    , pAutomatCreater_       ( 0 )
    , pUnitTeleporter_       ( 0 )
    , pRemoteAgentController_( 0 )
{
    CONNECT( messageController, *this, control_end_tick );
}

// -----------------------------------------------------------------------------
// Name: SimulationFacade destructor
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
SimulationFacade::~SimulationFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationFacade::Notify
// Created: SLI 2011-09-15
// -----------------------------------------------------------------------------
void SimulationFacade::Notify( const sword::ControlEndTick& /*message*/, int /*context*/ )
{
    DISCONNECT( messageController_, *this, control_end_tick );
    pAutomatDisengager_.reset( new AutomatDisengager( *pAutomatHandler_, publisher_, *pContextFactory_ ) );
    pFormationCreater_.reset( new FormationCreater( dynamicModel_.Sides(), *pFormationHandler_ ) );
    pAutomatCreater_.reset( new AutomatCreater( *pFormationHandler_, *pAutomatHandler_, staticModel_.types_, dynamicModel_.KnowledgeGroups() ) );
    pUnitTeleporter_.reset( new UnitTeleporter( remoteAgentSubject_, *pUnitHandler_, publisher_, *pContextFactory_ ) );
    pRemoteAgentController_.reset( new RemoteAgentController( remoteAgentSubject_, *pAutomatHandler_, *pUnitHandler_, dynamicModel_.Sides(), unitTypeResolver_ ) );
}
