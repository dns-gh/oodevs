// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_IndirectFire.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_IndirectFire constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_IndirectFire::MissionParameter_IndirectFire( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , fire_               ( asn.value.u.tirIndirect )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IndirectFire destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_IndirectFire::~MissionParameter_IndirectFire()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_IndirectFire::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_IndirectFire::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value          = bNullValue_;
    asn.value.t             = T_MissionParameter_value_tirIndirect;
    asn.value.u.tirIndirect = fire_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IndirectFire::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_IndirectFire::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
