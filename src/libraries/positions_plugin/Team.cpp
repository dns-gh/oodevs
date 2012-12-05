// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "Team.h"
#include "PositionsPlugin.h"
#include "Unit.h"

using namespace plugins::positions;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
Team::Team( unsigned int id, const std::string& name )
    : id_  ( id )
    , name_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
Team::~Team()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Team::AddUnit
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void Team::AddUnit( unsigned int id, const std::string& name, unsigned int timePreviouslyExported )
{
    Unit* unit = new Unit( id, name, timePreviouslyExported );
    Register( id, *unit );
}

// -----------------------------------------------------------------------------
// Name: Team::UpdatePosition
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
void Team::UpdatePosition( unsigned int unitId, const sword::CoordLatLong& coord )
{
    Unit* unit = Find( unitId );
    if( unit )
        unit->UpdatePosition( coord );
}

// -----------------------------------------------------------------------------
// Name: Team::Export
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void Team::Export( boost::filesystem::ofstream& file ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        file << name_ << " (" << id_ << ")" << PositionsPlugin::separator_;
        it->second->Export( file );
        file << std::endl;
    }
}
