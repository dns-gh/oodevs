// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_DotationType.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_DotationType constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_DotationType::MissionParameter_DotationType( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , dotationType_       ( asn.value.u.dotationType )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_DotationType destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_DotationType::~MissionParameter_DotationType()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_DotationType::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_DotationType::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value          = bNullValue_;
    asn.value.t              = T_MissionParameter_value_dotationType;
    asn.value.u.dotationType = dotationType_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_DotationType::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_DotationType::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
