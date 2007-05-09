// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Polygon.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Polygon constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Polygon::MissionParameter_Polygon( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , polygon_           ( *asn.value.u.polygon )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Polygon destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Polygon::~MissionParameter_Polygon()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Polygon::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Polygon::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value      = bNullValue_;
    asn.value.t         = T_MissionParameter_value_polygon;
    asn.value.u.polygon = new ASN1T_Polygon();
    polygon_.Send( *asn.value.u.polygon );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Polygon::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Polygon::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    Localisation::AsnDelete( *asn.value.u.polygon );
    delete asn.value.u.polygon;
}
