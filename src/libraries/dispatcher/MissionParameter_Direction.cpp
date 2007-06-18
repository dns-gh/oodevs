// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Direction.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Direction constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Direction::MissionParameter_Direction( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , direction_          ( asn.value.u.heading )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Direction destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Direction::~MissionParameter_Direction()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Direction::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Direction::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value      = bNullValue_;
    asn.value.t         = T_MissionParameter_value_heading;
    asn.value.u.heading = direction_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Direction::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Direction::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
