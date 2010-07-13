// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Position.h"
#include <geocoord/geodetic.h>
#include <geocoord/MGRS.h>

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
Position::Position()
    : latitude_ ( -89. )
    , longitude_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
Position::Position( xml::xistream& xis )
{
    xis >> xml::attribute( "latitude", latitude_ )
        >> xml::attribute( "longitude", longitude_ );
}

// -----------------------------------------------------------------------------
// Name: Position destructor
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
Position::~Position()
{
    // NOTHING
}

namespace
{
    const double factor = std::acos( -1.f ) / 180.;
}

// -----------------------------------------------------------------------------
// Name: Position::Write
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Position::Write( xml::xostream& xos ) const
{
    const double latitude = latitude_ * factor;
    const double longitude = longitude_ * factor;
    const geocoord::Geodetic geo( latitude, longitude );
    const geocoord::MGRS mgrs( geo );
    xos << xml::content( "point", mgrs.GetString() );
}

// -----------------------------------------------------------------------------
// Name: Position::WriteAttribute
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Position::WriteAttribute( const std::string& name, xml::xostream& xos ) const
{
    const double latitude = latitude_ * factor;
    const double longitude = longitude_ * factor;
    const geocoord::Geodetic geo( latitude, longitude );
    const geocoord::MGRS mgrs( geo );
    xos << xml::attribute( name, mgrs.GetString() );
}
