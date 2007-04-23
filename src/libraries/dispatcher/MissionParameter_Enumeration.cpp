// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Enumeration.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Enumeration constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Enumeration::MissionParameter_Enumeration( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , enumeration_        ( asn.value.u.enumeration )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Enumeration destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Enumeration::~MissionParameter_Enumeration()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Enumeration::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Enumeration::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value          = bNullValue_;
    asn.value.t             = T_MissionParameter_value_enumeration;
    asn.value.u.enumeration = enumeration_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Enumeration::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Enumeration::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
