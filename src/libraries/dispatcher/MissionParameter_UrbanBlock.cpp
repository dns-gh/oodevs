// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_UrbanBlock.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_UrbanBlock constructor
// Created: SLG 2009-11-06
// -----------------------------------------------------------------------------
MissionParameter_UrbanBlock::MissionParameter_UrbanBlock( const ASN1T_MissionParameter& asn )
: MissionParameter_ABC( asn )
, block_              ( asn.value.u.urbanBlock )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_UrbanBlock destructor
// Created: SLG 2009-11-06
// -----------------------------------------------------------------------------
MissionParameter_UrbanBlock::~MissionParameter_UrbanBlock()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_UrbanBlock::Send
// Created: SLG 2009-11-06
// -----------------------------------------------------------------------------
void MissionParameter_UrbanBlock::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value          = bNullValue_;
    asn.value.t             = T_MissionParameter_value_urbanBlock;
    asn.value.u.urbanBlock  = block_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_UrbanBlock::AsnDelete
// Created: SLG 2009-11-06
// -----------------------------------------------------------------------------
void MissionParameter_UrbanBlock::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
