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
#include "UnitTypeResolver_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/Team_ABC.h"
#include "clients_kernel/Karma.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteAgentController constructor
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
RemoteAgentController::RemoteAgentController( RemoteAgentSubject_ABC& agentSubject,
                                              ContextHandler_ABC< sword::AutomatCreation >& automatHandler,
                                              ContextHandler_ABC< sword::UnitCreation >& unitHandler,
                                              const tools::Resolver_ABC< dispatcher::Team_ABC >& sides,
                                              const UnitTypeResolver_ABC& typeResolver )
    : agentSubject_  ( agentSubject )
    , automatHandler_( automatHandler )
    , unitHandler_   ( unitHandler )
    , sides_         ( sides )
    , typeResolver_  ( typeResolver )
{
    automatHandler_.Register( *this );
    agentSubject_.Register( *this );
    for( tools::Iterator< const dispatcher::Team_ABC& > it = sides.CreateIterator(); it.HasMoreElements(); )
    {
        const dispatcher::Team_ABC& team = it.NextElement();
        karmas_[ team.GetKarma() ] = team.GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController destructor
// Created: SLI 2011-09-01
// -----------------------------------------------------------------------------
RemoteAgentController::~RemoteAgentController()
{
    automatHandler_.Unregister( *this );
    agentSubject_.Unregister( *this );
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
// Name: RemoteAgentController::Created
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentController::Created( const std::string& identifier )
{
    unitCreations_[ identifier ] = T_UnitCreation( new simulation::UnitMagicAction() );
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    message().set_type( sword::UnitMagicAction::unit_creation );
    message().mutable_parameters()->add_elem(); // type
    message().mutable_parameters()->add_elem(); // position
    message().mutable_parameters()->add_elem(); // name
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
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    sword::Location* location = message().mutable_parameters()->mutable_elem( 1 )->add_value()->mutable_point()->mutable_location();
    location->set_type( sword::Location::point );
    sword::CoordLatLong* coordLatLong = location->mutable_coordinates()->add_elem();
    coordLatLong->set_latitude( latitude );
    coordLatLong->set_longitude( longitude );
    Send( message, identifier );
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
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::FindAutomat
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
unsigned long RemoteAgentController::FindAutomat( rpr::ForceIdentifier force ) const
{
    T_Karmas::const_iterator itKarma = karmas_.find( GetKarma( force ) );
    if( itKarma == karmas_.end() )
        return 0;
    T_Parties::const_iterator itParty = parties_.find( itKarma->second );
    if( itParty == parties_.end() )
        return 0;
    return itParty->second;
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::NameChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void RemoteAgentController::NameChanged( const std::string& identifier, const std::string& name )
{
    if( unitCreations_.find( identifier ) == unitCreations_.end() )
        return;
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    message().mutable_parameters()->mutable_elem( 2 )->add_value()->set_acharstr( "HLA_" + name );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::TypeChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void RemoteAgentController::TypeChanged( const std::string& identifier, const rpr::EntityType& type )
{
    if( unitCreations_.find( identifier ) == unitCreations_.end() )
        return;
    simulation::UnitMagicAction& message = *unitCreations_[ identifier ];
    message().mutable_parameters()->mutable_elem( 0 )->add_value()->set_identifier( typeResolver_.Resolve( type ) );
    Send( message, identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentController::Send
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void RemoteAgentController::Send( simulation::UnitMagicAction& message, const std::string& identifier )
{
    if( message().has_tasker() &&
        message().parameters().elem( 1 ).value_size() > 0 &&
        message().parameters().elem( 2 ).value_size() > 0 )
    {
        unitHandler_.Send( message, identifier );
        unitCreations_.erase( identifier );
    }
}
