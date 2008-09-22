// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_String.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_String constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_String::MissionParameter_String( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , value_              ( asn.value.u.aCharStr )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_String destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_String::~MissionParameter_String()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_String::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_String::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value       = bNullValue_;
    asn.value.t          = T_MissionParameter_value_aCharStr;
    asn.value.u.aCharStr = value_.c_str();
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_String::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_String::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
