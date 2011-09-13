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
#include "ContextHandler_ABC.h"
#include "ContextFactory_ABC.h"
#include "protocol/Simulation.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/KnowledgeGroup_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/Karma.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteAgentController constructor
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
RemoteAgentController::RemoteAgentController( tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                              dispatcher::Model_ABC& model, tools::Resolver_ABC< kernel::AutomatType >& automatTypes,
                                              dispatcher::SimulationPublisher_ABC& publisher, RemoteAgentSubject_ABC& agentSubject,
                                              const ContextFactory_ABC& contextFactory,
                                              ContextHandler_ABC< sword::FormationCreation >& formationHandler,
                                              ContextHandler_ABC< sword::AutomatCreation >& automatHandler,
                                              ContextHandler_ABC< sword::UnitCreation >& unitHandler )
    : controller_       ( controller )
    , model_            ( model )
    , publisher_        ( publisher )
    , agentSubject_     ( agentSubject )
    , automatType_      ( 230u ) // $$$$ _RC_ SLI 2011-09-07: hardcoded
    , contextFactory_   ( contextFactory )
    , formationHandler_ ( formationHandler )
    , automatHandler_   ( automatHandler )
    , unitHandler_      ( unitHandler )
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
    unitHandler_.Unregister( *this );
    automatHandler_.Unregister( *this );
    formationHandler_.Unregister( *this );
    agentSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Notify
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::Notify( const sword::ControlEndTick& /*message*/, int /*context*/ )
{
    DISCONNECT( controller_, *this, control_end_tick );
    formationHandler_.Register( *this );
    automatHandler_.Register( *this );
    unitHandler_.Register( *this );
    tools::Iterator< const dispatcher::Team_ABC& > it = model_.Sides().CreateIterator();
    while( it.HasMoreElements() )
    {
        const dispatcher::Team_ABC& team = it.NextElement();
        AddFormation( team.GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Notify
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::Notify( const sword::FormationCreation& message, const std::string& /*identifier*/ )
{
    AddAutomat( message.formation().id(), FindKnowledgeGroup( message.party().id() ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Notify
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::Notify( const sword::AutomatCreation& message, const std::string& /*identifier*/ )
{
    parties_[ message.party().id() ] = message.automat().id();
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Notify
// Created: SLI 2011-09-08
// -----------------------------------------------------------------------------
void RemoteAgentController::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    units_[ identifier ] = message.unit().id();
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
    formationHandler_.Send( message, boost::lexical_cast< std::string >( party ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::AddAutomat
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
void RemoteAgentController::AddAutomat( unsigned long formation, unsigned long knowledgeGroup )
{
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_formation()->set_id( formation );                           // parent
    message().set_type( sword::UnitMagicAction::automat_creation );
    message().mutable_parameters()->add_elem()->add_value()->set_identifier( automatType_ );        // type
    message().mutable_parameters()->add_elem()->add_value()->set_identifier( knowledgeGroup );      // knowledge group
    message().mutable_parameters()->add_elem()->add_value()->set_acharstr( "HLA distant automat" ); // name
    automatHandler_.Send( message, boost::lexical_cast< std::string >( formation ) );
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
    unitCreations_[ identifier ] = T_UnitCreation( new simulation::UnitMagicAction() );
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    message().set_type( sword::UnitMagicAction::unit_creation );
    message().mutable_parameters()->add_elem()->add_value()->set_identifier( 64 ); // $$$$ _RC_ VPR 2011-09-07: Hardcoded
    message().mutable_parameters()->add_elem();                                                   // position
    message().mutable_parameters()->add_elem()->add_value()->set_acharstr( "HLA_" + identifier ); // name
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
    if( units_.find( identifier ) != units_.end() )
        Teleport( identifier, latitude, longitude );
    else if( unitCreations_.find( identifier ) != unitCreations_.end() )
    {
        simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
        sword::Location* location = message().mutable_parameters()->mutable_elem( 1 )->add_value()->mutable_point()->mutable_location();
        location->set_type( sword::Location::point );
        sword::CoordLatLong* coordLatLong = location->mutable_coordinates()->add_elem();
        coordLatLong->set_latitude( latitude );
        coordLatLong->set_longitude( longitude );
        if( message().has_tasker() )
        {
            unitHandler_.Send( message, identifier );
            unitCreations_.erase( identifier );
        }
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
// Name: RemoteAgentController::SideChanged
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::SideChanged( const std::string& identifier, rpr::ForceIdentifier side )
{
    if( unitCreations_.find( identifier ) == unitCreations_.end() )
        return;
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    const unsigned long automat = FindAutomat( side );
    if( automat == 0 )
        throw std::runtime_error( "Army '" + GetKarma( side ).GetName().toStdString() + "' does not exist for remote agent '" + identifier + "'" );
    message().mutable_tasker()->mutable_automat()->set_id( automat );
    if( message().parameters().elem( 1 ).value_size() > 0 )
    {
        unitHandler_.Send( message, identifier );
        unitCreations_.erase( identifier );
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

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Teleport
// Created: SLI 2011-09-08
// -----------------------------------------------------------------------------
void RemoteAgentController::Teleport( const std::string& identifier, double latitude, double longitude )
{
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_unit()->set_id( units_[ identifier ] );
    message().set_type( sword::UnitMagicAction::move_to );
    sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
    parameter.set_null_value( false );
    sword::Location& location = *parameter.add_value()->mutable_point()->mutable_location();
    location.set_type( sword::Location::point );
    sword::CoordLatLong& coordinates = *location.mutable_coordinates()->add_elem();
    coordinates.set_latitude( latitude );
    coordinates.set_longitude( longitude );
    message.Send( publisher_, contextFactory_.Create() );
}
