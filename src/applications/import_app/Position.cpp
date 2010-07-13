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
// Name: operator<<
// Created: MCO 2010-07-13
// -----------------------------------------------------------------------------
xml::xostream& operator<<( xml::xostream& xos, const Position& position )
{
    const double latitude = position.latitude_ * factor;
    const double longitude = position.longitude_ * factor;
    const geocoord::Geodetic geo( latitude, longitude );
    return xos << geocoord::MGRS( geo ).GetString();
}
