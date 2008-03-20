// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPoint.h"
#include "game_asn/Simulation.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterPoint constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPoint::ActionParameterPoint( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : ActionParameterLocation( parameter, converter, location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPoint constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPoint::ActionParameterPoint( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Point& asn )
    : ActionParameterLocation( parameter, converter, asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPoint constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPoint::ActionParameterPoint( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameterLocation( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPoint destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPoint::~ActionParameterPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPoint::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPoint::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_point;
    asn.value.u.point = new ASN1T_Point();
    if( IsSet() )
        ActionParameterLocation::CommitTo( *asn.value.u.point );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPoint::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPoint::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.point )
        ActionParameterLocation::Clean( *asn.value.u.point );
    delete asn.value.u.point;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPoint::CommitTo
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterPoint::CommitTo( ASN1T_CoordUTM& asn ) const
{
    ASN1T_Location loc;
    ActionParameterLocation::CommitTo( loc );
    asn = loc.coordinates.elem[0];
    ActionParameterLocation::Clean( loc );
}
