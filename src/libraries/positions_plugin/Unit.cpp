// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "Unit.h"
#include "PositionsPlugin.h"
#include "protocol/Protocol.h"

using namespace plugins::positions;

// -----------------------------------------------------------------------------
// Name: Unit constructor
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
Unit::Unit( unsigned int id, const std::string& name, unsigned int timePreviouslyExported )
    : id_  ( id )
    , name_( name )
{
    for( unsigned int i = 0; i <= timePreviouslyExported; ++i )
        positions_.push_back( "NA" );
}

// -----------------------------------------------------------------------------
// Name: Unit destructor
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
Unit::~Unit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Unit::UpdatePosition
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
void Unit::UpdatePosition( const sword::CoordLatLong& coord )
{
    std::stringstream pos;
    pos << "Lat:" << coord.latitude() << " Long:" << coord.longitude();
    positions_[ positions_.size() - 1 ] = pos.str();
}

// -----------------------------------------------------------------------------
// Name: Unit::Export
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void Unit::Export( boost::filesystem::ofstream& file )
{
    file << name_ << " (" << id_ << ")";
    for( auto it = positions_.begin(); it != positions_.end(); ++it )
        file << PositionsPlugin::separator_ << *it;
    positions_.push_back( *positions_.rbegin() );
}
