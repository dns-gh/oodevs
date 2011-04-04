// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "TeamResolver.h"
#include "Team.h"
#include <boost/bind/bind.hpp>
#include "protocol/Protocol.h"

using namespace plugins::positions;

// -----------------------------------------------------------------------------
// Name: TeamResolver constructor
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
TeamResolver::TeamResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamResolver destructor
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
TeamResolver::~TeamResolver()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TeamResolver::AddTeam
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void TeamResolver::AddTeam( unsigned int id, const std::string& name )
{
    Team* team = new Team( id, name );
    Register( id, *team );
}

// -----------------------------------------------------------------------------
// Name: TeamResolver::AddAutomat
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
void TeamResolver::AddAutomat( unsigned int teamId, unsigned int automatId )
{
    Team* team = Find( teamId );
    if( !team )
        throw std::runtime_error( __FUNCTION__ "unknown team." );
    automats_[ automatId ] = team;
}

// -----------------------------------------------------------------------------
// Name: TeamResolver::AddUnit
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
void TeamResolver::AddUnit( unsigned int automatId, unsigned int unitId, const std::string& name, unsigned int timePreviouslyExported )
{
    CIT_Automats it = automats_.find( automatId );

    if( it == automats_.end() )
        throw std::runtime_error( __FUNCTION__ "unknown automat." );

    it->second->AddUnit( unitId, name, timePreviouslyExported );
}

// -----------------------------------------------------------------------------
// Name: TeamResolver::UpdatePosition
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void TeamResolver::UpdatePosition( unsigned int unitId, const sword::CoordLatLong& coord )
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        Team* team = it->second;
        team->UpdatePosition( unitId, coord );
    }
}

// -----------------------------------------------------------------------------
// Name: TeamResolver::Export
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void TeamResolver::Export( boost::filesystem::ofstream& file ) const
{
    Apply( boost::bind( &Team::Export, _1, boost::ref( file ) ) );
}
