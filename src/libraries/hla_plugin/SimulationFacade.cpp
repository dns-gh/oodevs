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
SimulationFacade::SimulationFacade( const ContextFactory_ABC& contextFactory, tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                                    dispatcher::SimulationPublisher_ABC& publisher, dispatcher::Model_ABC& dynamicModel,
                                    const dispatcher::StaticModel& staticModel, const UnitTypeResolver_ABC& unitTypeResolver,
                                    RemoteAgentSubject_ABC& remoteAgentSubject, const LocalAgentResolver_ABC& localAgentResolver )
    : contextFactory_        ( contextFactory )
    , messageController_     ( messageController )
    , publisher_             ( publisher )
    , dynamicModel_          ( dynamicModel )
    , staticModel_           ( staticModel )
    , unitTypeResolver_      ( unitTypeResolver )
    , remoteAgentSubject_    ( remoteAgentSubject )
    , localAgentResolver_    ( localAgentResolver )
    , pFormationHandler_     ( new FormationContextHandler( messageController, contextFactory, publisher ) )
    , pAutomatHandler_       ( new AutomatContextHandler( messageController, contextFactory, publisher ) )
    , pUnitHandler_          ( new UnitContextHandler( messageController, contextFactory, publisher ) )
    , pAutomatDisengager_    ( new AutomatDisengager( *pAutomatHandler_, publisher_, contextFactory_ ) )
    , pFormationCreater_     ( new FormationCreater( dynamicModel_.Sides(), *pFormationHandler_ ) )
    , pAutomatCreater_       ( new AutomatCreater( *pFormationHandler_, *pAutomatHandler_, staticModel_.types_, dynamicModel_.KnowledgeGroups() ) )
    , pUnitTeleporter_       ( new UnitTeleporter( remoteAgentSubject_, *pUnitHandler_, publisher_, contextFactory_ ) )
    , pRemoteAgentController_( new RemoteAgentController( remoteAgentSubject_, *pAutomatHandler_, *pUnitHandler_, dynamicModel_.Sides(), unitTypeResolver_ ) )
{
    // NOTHING
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
// Name: SimulationFacade::Register
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void SimulationFacade::Register( ResponseObserver_ABC< sword::UnitCreation >& observer )
{
    pUnitHandler_->Register( observer );
}

// -----------------------------------------------------------------------------
// Name: SimulationFacade::Unregister
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void SimulationFacade::Unregister( ResponseObserver_ABC< sword::UnitCreation >& observer )
{
    pUnitHandler_->Unregister( observer );
}

// -----------------------------------------------------------------------------
// Name: SimulationFacade::Send
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void SimulationFacade::Send( simulation::UnitMagicAction& message, const std::string& identifier )
{
    pUnitHandler_->Send( message, identifier );
}
