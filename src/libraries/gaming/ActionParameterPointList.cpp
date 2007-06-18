// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPointList.h"
#include "game_asn/Asn.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterPointList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPointList::ActionParameterPointList( const OrderParameter& parameter )
    : ActionParameterLocationList( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPointList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPointList::ActionParameterPointList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_PointList& asn )
    : ActionParameterLocationList( parameter, converter, (const ASN1T_LocationList&)asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPointList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPointList::ActionParameterPointList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameterLocationList( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPointList destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPointList::~ActionParameterPointList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPointList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPointList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_pointList;
    asn.value.u.pointList = new ASN1T_PointList();
    ActionParameterLocationList::CommitTo( *(ASN1T_LocationList*)asn.value.u.pointList );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPointList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPointList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.pointList )
        ActionParameterLocationList::Clean( *(ASN1T_LocationList*)asn.value.u.pointList );
    delete asn.value.u.pointList;
}
