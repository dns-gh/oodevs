// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Numeric.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Numeric constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Numeric::MissionParameter_Numeric( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , value_              ( asn.value.u.aReal )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Numeric destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Numeric::~MissionParameter_Numeric()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Numeric::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Numeric::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value    = bNullValue_;
    asn.value.t       = T_MissionParameter_value_aReal;
    asn.value.u.aReal = value_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Numeric::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Numeric::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
