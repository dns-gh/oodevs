// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Agent.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Agent constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Agent::MissionParameter_Agent( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , agent_              ( asn.value.u.unit )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Agent destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Agent::~MissionParameter_Agent()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Agent::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Agent::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value    = bNullValue_;
    asn.value.t       = T_MissionParameter_value_unit;
    asn.value.u.unit = agent_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Agent::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Agent::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
