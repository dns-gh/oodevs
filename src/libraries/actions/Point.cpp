// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Point.h"
#include "game_asn/Simulation.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Point::Point( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : Location( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Point::Point( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Point& asn )
    : Location( parameter, converter, asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Point::Point( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Location( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Point::~Point()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Point::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_point;
    asn.value.u.point = new ASN1T_Point();
    if( IsSet() )
        Location::CommitTo( *asn.value.u.point );
}

// -----------------------------------------------------------------------------
// Name: Point::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Point::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.point )
        Location::Clean( *asn.value.u.point );
    delete asn.value.u.point;
}

// -----------------------------------------------------------------------------
// Name: Point::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Point::CommitTo( ASN1T_CoordLatLong& asn ) const
{
    ASN1T_Location loc;
    Location::CommitTo( loc );
    asn = loc.coordinates.elem[0];
    Location::Clean( loc );
}
