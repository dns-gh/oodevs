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
#include "NetnRemoteCallsignListener.h"
#include "AutomatTypeResolver.h"
#include "EquipmentUpdater.h"
#include "protocol/Simulation.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/StaticModel.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include <xeumeuleu/xml.hpp>

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
SimulationFacade::SimulationFacade( xml::xisubstream xis, const ContextFactory_ABC& contextFactory, tools::MessageController_ABC< sword::SimToClient_Content >& messageController,
                                    dispatcher::SimulationPublisher_ABC& publisher, dispatcher::Model_ABC& dynamicModel, const rpr::EntityTypeResolver_ABC& componentTypeResolver,
                                    const dispatcher::StaticModel& staticModel, const UnitTypeResolver_ABC& unitTypeResolver,
                                    RemoteAgentSubject_ABC& remoteAgentSubject, const ComponentTypes_ABC& componentTypes, CallsignResolver_ABC& callsignResolver )
    : pFormationHandler_          ( new FormationContextHandler( messageController, contextFactory, publisher ) )
    , pAutomatHandler_            ( new AutomatContextHandler( messageController, contextFactory, publisher ) )
    , pUnitHandler_               ( new UnitContextHandler( messageController, contextFactory, publisher ) )
    , pAutomatTypeResolver_       ( new AutomatTypeResolver( staticModel.types_ ) )
    , pAutomatDisengager_         ( new AutomatDisengager( *pAutomatHandler_, publisher, contextFactory ) )
    , pFormationCreater_          ( new FormationCreater( dynamicModel.Sides(), *pFormationHandler_ ) )
    , pAutomatCreater_            ( new AutomatCreater( xis, *pFormationHandler_, *pAutomatHandler_, *pAutomatTypeResolver_, dynamicModel.KnowledgeGroups() ) )
    , pUnitTeleporter_            ( new UnitTeleporter( remoteAgentSubject, *pUnitHandler_, publisher, contextFactory ) )
    , pEquipmentUpdater_          ( new EquipmentUpdater( remoteAgentSubject, *pUnitHandler_, publisher, contextFactory, componentTypeResolver, componentTypes, messageController ) )
    , pRemoteAgentController_     ( new RemoteAgentController( remoteAgentSubject, *pAutomatHandler_, *pUnitHandler_, dynamicModel.Sides(), unitTypeResolver ) )
    , pNetnRemoteCallsignListener_( new NetnRemoteCallsignListener( callsignResolver, remoteAgentSubject, *pUnitHandler_ ) )
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
