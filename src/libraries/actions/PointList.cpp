// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PointList.h"
#include "game_asn/Simulation.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PointList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PointList::PointList( const OrderParameter& parameter )
    : LocationList( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PointList::PointList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_PointList& asn )
    : LocationList( parameter, converter, (const ASN1T_LocationList&)asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointList constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PointList::PointList( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, xml::xistream& xis )
    : LocationList( parameter, converter, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointList destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
PointList::~PointList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PointList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_pointList;
    asn.value.u.pointList = new ASN1T_PointList();
    if( IsSet() )
        LocationList::CommitTo( *(ASN1T_LocationList*)asn.value.u.pointList );
}

// -----------------------------------------------------------------------------
// Name: PointList::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void PointList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.pointList )
        LocationList::Clean( *(ASN1T_LocationList*)asn.value.u.pointList );
    delete asn.value.u.pointList;
}
