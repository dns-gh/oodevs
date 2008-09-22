// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Automat.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Automat constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Automat::MissionParameter_Automat( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , automat_            ( asn.value.u.automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Automat destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Automat::~MissionParameter_Automat()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Automat::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Automat::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value       = bNullValue_;
    asn.value.t          = T_MissionParameter_value_automat;
    asn.value.u.automat = automat_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Automat::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Automat::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
