// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRemoteCallsignListener.h"
#include "RemoteAgentSubject_ABC.h"
#include "CallsignResolver_ABC.h"
#include "ContextHandler_ABC.h"
#include "protocol/Simulation.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
NetnRemoteCallsignListener::NetnRemoteCallsignListener( CallsignResolver_ABC& resolver, RemoteAgentSubject_ABC& subject,
                                                        ContextHandler_ABC< sword::UnitCreation >& handler )
    : resolver_( resolver )
    , subject_ ( subject )
    , handler_ ( handler )
{
    subject_.Register( *this );
    handler_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener destructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
NetnRemoteCallsignListener::~NetnRemoteCallsignListener()
{
    handler_.Unregister( *this );
    subject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Add
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Add( const std::string& identifier )
{
    T_Add& add = adds_[ identifier ];
    if( add.simulationIdentifier != 0 && !add.callsign.empty() && !add.uniqueId.empty() )
    {
        resolver_.Add( add.simulationIdentifier, add.callsign, add.uniqueId );
        adds_.erase( identifier );
    }
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::UniqueIdChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::UniqueIdChanged( const std::string& identifier, const std::string& uniqueId )
{
    adds_[ identifier ].uniqueId = uniqueId;
    Add( identifier );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::CallsignChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::CallsignChanged( const std::string& identifier, const std::string& callsign )
{
    adds_[ identifier ].callsign = callsign;
    Add( identifier );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Notify
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Notify( const sword::UnitCreation& message, const std::string& identifier )
{
    adds_[ identifier ].simulationIdentifier = message.unit().id();
    Add( identifier );
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Created
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Created( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Destroyed
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Destroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::Moved
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::Moved( const std::string& /*identifier*/, double /*latitude*/, double /*longitude*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::SideChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::SideChanged( const std::string& /*identifier*/, rpr::ForceIdentifier /*side*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::NameChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::TypeChanged
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteCallsignListener::EquipmentUpdated
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void NetnRemoteCallsignListener::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*number*/ )
{
    // NOTHING
}
