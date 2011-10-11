// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "SideChecker.h"
#include "AgentSubject_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "RemoteAgentResolver_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: SideChecker constructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
SideChecker::SideChecker( AgentSubject_ABC& localSubject, RemoteAgentSubject_ABC& remoteSubject, const RemoteAgentResolver_ABC& remoteResolver )
    : localSubject_  ( localSubject )
    , remoteSubject_ ( remoteSubject )
    , remoteResolver_( remoteResolver )
{
    localSubject_.Register( *this );
    remoteSubject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SideChecker destructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
SideChecker::~SideChecker()
{
    remoteSubject_.Unregister( *this );
    localSubject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SideChecker::AreSameSide
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
bool SideChecker::AreSameSide( unsigned long firstIdentifier, unsigned long secondIdentifier ) const
{
    const int firstSide = Find( firstIdentifier );
    const int secondSide = Find( secondIdentifier );
    if( firstSide < 0 || secondSide < 0 )
        return false;
    return firstSide == secondSide;
}

// -----------------------------------------------------------------------------
// Name: SideChecker::Find
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
int SideChecker::Find( unsigned int identifier ) const
{
    T_LocalSides::const_iterator local = localSides_.find( identifier );
    if( local == localSides_.end() )
    {
        const std::string name = remoteResolver_.Resolve( identifier );
        if( name.empty() )
            return -1;
        T_RemoteSides::const_iterator remote = remoteSides_.find( name );
        if( remote == remoteSides_.end() )
            return -1;
        return remote->second;
    }
    return local->second;
}

// -----------------------------------------------------------------------------
// Name: SideChecker::AggregateCreated
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::AggregateCreated( Agent_ABC& /*agent*/, unsigned int identifier, const std::string& /*name*/, rpr::ForceIdentifier force, const rpr::EntityType& /*type*/ )
{
    localSides_[ identifier ] = force;
}

// -----------------------------------------------------------------------------
// Name: SideChecker::SurfaceVesselCreated
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::SurfaceVesselCreated( Agent_ABC& /*agent*/, unsigned int identifier, const std::string& /*name*/, rpr::ForceIdentifier force, const rpr::EntityType& /*type*/ )
{
    localSides_[ identifier ] = force;
}

// -----------------------------------------------------------------------------
// Name: SideChecker::SideChanged
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::SideChanged( const std::string& identifier, rpr::ForceIdentifier side )
{
    remoteSides_[ identifier ] = side;
}

// -----------------------------------------------------------------------------
// Name: SideChecker::Created
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::Created( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideChecker::Destroyed
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::Destroyed( const std::string& /*identifier*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideChecker::Moved
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::Moved( const std::string& /*identifier*/, double /*latitude*/, double /*longitude*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideChecker::NameChanged
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::NameChanged( const std::string& /*identifier*/, const std::string& /*name*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideChecker::TypeChanged
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::TypeChanged( const std::string& /*identifier*/, const rpr::EntityType& /*type*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideChecker::EquipmentUpdated
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::EquipmentUpdated( const std::string& /*identifier*/, const rpr::EntityType& /*equipmentType*/, unsigned int /*number*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideChecker::UniqueIdChanged
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::UniqueIdChanged( const std::string& /*identifier*/, const std::string& /*uniqueId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideChecker::CallsignChanged
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void SideChecker::CallsignChanged( const std::string& /*identifier*/, const std::string& /*callsign*/ )
{
    // NOTHING
}
