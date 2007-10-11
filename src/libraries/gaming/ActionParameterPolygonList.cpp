// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPolygonList.h"
#include "game_asn/Asn.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygonList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPolygonList::ActionParameterPolygonList( const OrderParameter& parameter )
    : ActionParameterLocationList( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygonList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPolygonList::ActionParameterPolygonList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_PolygonList& asn )
    : ActionParameterLocationList( parameter, converter, (const ASN1T_LocationList&)asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygonList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPolygonList::ActionParameterPolygonList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameterLocationList( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygonList destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterPolygonList::~ActionParameterPolygonList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygonList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPolygonList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = Count() ? 0 : 1;
    asn.value.t = T_MissionParameter_value_polygonList;
    asn.value.u.polygonList = new ASN1T_PolygonList();
    ActionParameterLocationList::CommitTo( *(ASN1T_LocationList*)asn.value.u.polygonList );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPolygonList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPolygonList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.polygonList )
        ActionParameterLocationList::Clean( *(ASN1T_LocationList*)asn.value.u.polygonList );
    delete asn.value.u.polygonList;
}
