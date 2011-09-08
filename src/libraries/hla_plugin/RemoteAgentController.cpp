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
#include "RemoteAgentSubject_ABC.h"
#include "protocol/Simulation.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/KnowledgeGroup_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/Karma.h"
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
                                              dispatcher::Model_ABC& model, tools::Resolver_ABC< kernel::AutomatType >& automatTypes,
                                              dispatcher::SimulationPublisher_ABC& publisher, RemoteAgentSubject_ABC& agentSubject )
    : controller_  ( controller )
    , model_       ( model )
    , publisher_   ( publisher )
    , agentSubject_( agentSubject )
    , automatType_ ( 230u ) // $$$$ _RC_ SLI 2011-09-07: hardcoded
{
    if( automatTypes.Find( automatType_ ) == 0 )
        throw std::runtime_error( "Automat type identifier '" + boost::lexical_cast< std::string >( automatType_ ) + "' not found, please check your physical model." );
    CONNECT( controller, *this, control_end_tick );
    agentSubject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController destructor
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
RemoteAgentController::~RemoteAgentController()
{
    agentSubject_.Unregister( *this );
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
void RemoteAgentController::Notify( const sword::AutomatCreation& message, int context )
{
    if( contexts_.find( context ) != contexts_.end() )
    {
        parties_[ message.party().id() ] = message.automat().id();
        contexts_.erase( context );
    }
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

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Created
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::Created( const std::string& identifier )
{
    unitCreations_[ identifier ] = T_UnitCreation( new simulation::UnitCreationRequest() );
    (*unitCreations_[ identifier ])().mutable_type()->set_id( 64 ); // $$$$ _RC_ VPR 2011-09-07: Hardcoded
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Destroyed
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::Destroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Moved
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::Moved( const std::string& identifier, double latitude, double longitude )
{
    if( unitCreations_.find( identifier ) == unitCreations_.end() )
        return;
    simulation::UnitCreationRequest& message = *unitCreations_[ identifier ];
    message().mutable_position()->set_latitude( latitude );
    message().mutable_position()->set_longitude( longitude );
    if( message().has_superior() )
    {
        message.Send( publisher_, *contexts_.insert( MakeContext() ).first );
        unitCreations_.erase( identifier );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::SideChanged
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::SideChanged( const std::string& identifier, rpr::ForceIdentifier side )
{
    if( unitCreations_.find( identifier ) == unitCreations_.end() )
        return;
    simulation::UnitCreationRequest& message = *unitCreations_[ identifier ];
    message().mutable_superior()->set_id( FindAutomat( side ) );
    if( message().has_position() )
    {
        message.Send( publisher_, *contexts_.insert( MakeContext() ).first );
        unitCreations_.erase( identifier );
    }
}

namespace
{
    const kernel::Karma& GetKarma( rpr::ForceIdentifier force )
    {
        if( force == rpr::Friendly )
            return kernel::Karma::friend_;
        if( force == rpr::Neutral )
            return kernel::Karma::neutral_;
        if( force == rpr::Opposing )
            return kernel::Karma::enemy_;
        return kernel::Karma::unknown_;
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::FindAutomat
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
unsigned long RemoteAgentController::FindAutomat( rpr::ForceIdentifier force ) const
{
    const kernel::Karma& karma = GetKarma( force );
    tools::Iterator< const dispatcher::Team_ABC& > it = model_.Sides().CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::Team_ABC& team = it.NextElement();
        T_Parties::const_iterator it = parties_.find( team.GetId() );
        if( team.GetKarma() == karma && it != parties_.end() )
            return it->second;
    }
    return 0;
}
