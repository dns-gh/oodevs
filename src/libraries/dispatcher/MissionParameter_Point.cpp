// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Point.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Point constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Point::MissionParameter_Point( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , point_              ( *asn.value.u.point )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Point destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Point::~MissionParameter_Point()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Point::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Point::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value    = bNullValue_;
    asn.value.t       = T_MissionParameter_value_tirIndirect;
    asn.value.u.point = new ASN1T_Localisation();
    point_.Send( *asn.value.u.point );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Point::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Point::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    Localisation::AsnDelete( *asn.value.u.point );
    delete asn.value.u.point;
}
