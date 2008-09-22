// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Bool.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Bool constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Bool::MissionParameter_Bool( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , bValue_             ( asn.value.u.aBool )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Bool destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Bool::~MissionParameter_Bool()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Bool::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Bool::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value    = bNullValue_;
    asn.value.t       = T_MissionParameter_value_aBool;
    asn.value.u.aBool = bValue_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Bool::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Bool::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
