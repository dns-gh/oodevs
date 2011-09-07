// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteAgentController.h"
#include "protocol/Simulation.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/KnowledgeGroup_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

namespace
{
    int MakeContext()
    {
        static int context = 0;
        return ++context;
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController constructor
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
RemoteAgentController::RemoteAgentController( tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                              dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& publisher )
    : controller_( controller )
    , model_     ( model )
    , publisher_ ( publisher )
    , automatType_( 230u ) // $$$$ _RC_ SLI 2011-09-07: hardcoded
{
    if( model_.Automats().Find( automatType_ ) == 0 )
        throw std::runtime_error( "Automat type identifier '" + boost::lexical_cast< std::string >( automatType_ ) + "' not found, please check your physical model." );
    CONNECT( controller, *this, control_end_tick );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController destructor
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
RemoteAgentController::~RemoteAgentController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Notify
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::Notify( const sword::ControlEndTick& /*message*/, int /*context*/ )
{
    tools::Iterator< const dispatcher::Team_ABC& > it = model_.Sides().CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::Team_ABC& team = it.NextElement();
        AddFormation( team.GetId() );
    }
    DISCONNECT( controller_, *this, control_end_tick );
    CONNECT( controller_, *this, formation_creation );
    CONNECT( controller_, *this, automat_creation );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Notify
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::Notify( const sword::FormationCreation& message, int context )
{
    if( contexts_.find( context ) != contexts_.end() )
    {
        AddAutomat( message.formation().id(), FindKnowledgeGroup( message.party().id() ) );
        contexts_.erase( context );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Notify
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::Notify( const sword::AutomatCreation& /*message*/, int context )
{
    contexts_.erase( context );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::AddFormation
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::AddFormation( unsigned long party )
{
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_party()->set_id( party );
    message().set_type( sword::UnitMagicAction::formation_creation );
    message().mutable_parameters()->add_elem()->add_value()->set_areal( 6 );                          // hierarchy level
    message().mutable_parameters()->add_elem()->add_value()->set_acharstr( "HLA distant formation" ); // name
    message().mutable_parameters()->add_elem()->set_null_value( true );                               // logistic level
    message.Send( publisher_, *contexts_.insert( MakeContext() ).first );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::AddAutomat
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::AddAutomat( unsigned long formation, unsigned long knowledgeGroup )
{
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_formation()->set_id( formation );                      // parent
    message().set_type( sword::UnitMagicAction::automat_creation );
    message().mutable_parameters()->add_elem()->add_value()->set_identifier( automatType_ );            // type
    message().mutable_parameters()->add_elem()->add_value()->set_identifier( knowledgeGroup ); // knowledge group
    message.Send( publisher_, *contexts_.insert( MakeContext() ).first );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::FindKnowledgeGroup
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
unsigned long RemoteAgentController::FindKnowledgeGroup( unsigned long party ) const
{
    tools::Iterator< const dispatcher::KnowledgeGroup_ABC& > it = model_.KnowledgeGroups().CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::KnowledgeGroup_ABC& knowledgeGroup = it.NextElement();
        if( knowledgeGroup.GetTeam().GetId() == party )
            return knowledgeGroup.GetId();
    }
    return 0;
}
