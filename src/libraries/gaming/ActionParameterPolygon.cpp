// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPolygon.h"
#include "game_asn/Simulation.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygon constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPolygon::ActionParameterPolygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : ActionParameterLocation( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygon constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPolygon::ActionParameterPolygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Polygon& asn )
    : ActionParameterLocation( parameter, converter, asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygon constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPolygon::ActionParameterPolygon( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameterLocation( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygon destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPolygon::~ActionParameterPolygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygon::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPolygon::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_polygon;
    asn.value.u.polygon = new ASN1T_Polygon();
    ActionParameterLocation::CommitTo( *asn.value.u.polygon );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygon::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPolygon::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.polygon )
        ActionParameterLocation::Clean( *asn.value.u.polygon );
    delete asn.value.u.polygon;
}
