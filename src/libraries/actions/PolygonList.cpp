// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PolygonList.h"
#include "game_asn/Simulation.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PolygonList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PolygonList::PolygonList( const OrderParameter& parameter )
    : LocationList( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PolygonList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PolygonList::PolygonList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_PolygonList& asn )
    : LocationList( parameter, converter, (const ASN1T_LocationList&)asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PolygonList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PolygonList::PolygonList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : LocationList( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PolygonList destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PolygonList::~PolygonList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PolygonList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PolygonList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_polygonList;
    asn.value.u.polygonList = new ASN1T_PolygonList();
    if( IsSet() )
        LocationList::CommitTo( *(ASN1T_LocationList*)asn.value.u.polygonList );
}

// -----------------------------------------------------------------------------
// Name: PolygonList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PolygonList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.polygonList )
        LocationList::Clean( *(ASN1T_LocationList*)asn.value.u.polygonList );
    delete asn.value.u.polygonList;
}
