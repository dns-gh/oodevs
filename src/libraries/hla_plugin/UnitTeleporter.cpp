// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "UnitTeleporter.h"
#include "ContextHandler_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "ContextFactory_ABC.h"
#include "protocol/SimulationSenders.h"
#include "dispatcher/SimulationPublisher_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: UnitTeleporter constructor
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
UnitTeleporter::UnitTeleporter( RemoteAgentSubject_ABC& agentSubject, ContextHandler_ABC< sword::UnitCreation >& contextHandler,
                                dispatcher::SimulationPublisher_ABC& publisher, const ContextFactory_ABC& contextFactory )
    : agentSubject_  ( agentSubject )
    , contextHandler_( contextHandler )
    , publisher_     ( publisher )
    , contextFactory_( contextFactory )
{
    agentSubject_.Register( *this );
    contextHandler_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter destructor
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
UnitTeleporter::~UnitTeleporter()
{
    contextHandler_.Unregister( *this );
    agentSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Created
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::Created( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Destroyed
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::Destroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Moved
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::Moved( const std::string& identifier, double latitude, double longitude )
{
    if( identifiers_.find( identifier ) == identifiers_.end() )
        return;
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_unit()->set_id( identifiers_[ identifier ] );
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

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::SideChanged
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::Notify
// Created: SLI 2011-09-13
// -----------------------------------------------------------------------------
void UnitTeleporter::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    identifiers_[ identifier ] = message.unit().id();
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::NameChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void UnitTeleporter::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::TypeChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void UnitTeleporter::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::EquipmentUpdated
// Created: SLI 2011-09-29
// -----------------------------------------------------------------------------
void UnitTeleporter::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*number*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::UniqueIdChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void UnitTeleporter::UniqueIdChanged( const std::string& /*identifier*/, const std::string& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitTeleporter::CallsignChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void UnitTeleporter::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}
