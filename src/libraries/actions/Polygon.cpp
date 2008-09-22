// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Polygon.h"
#include "game_asn/Simulation.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Polygon::Polygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : Location( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Polygon::Polygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Polygon& asn )
    : Location( parameter, converter, asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Polygon::Polygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : Location( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Polygon::~Polygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Polygon::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_polygon;
    asn.value.u.polygon = new ASN1T_Polygon();
    if( IsSet() )
        Location::CommitTo( *asn.value.u.polygon );
}

// -----------------------------------------------------------------------------
// Name: Polygon::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Polygon::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.polygon )
        Location::Clean( *asn.value.u.polygon );
    delete asn.value.u.polygon;
}
