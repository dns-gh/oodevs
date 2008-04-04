// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Location.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Location constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Location::MissionParameter_Location( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , location_           ( *asn.value.u.location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Location destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Location::~MissionParameter_Location()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Location::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Location::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value           = bNullValue_;
    asn.value.t              = T_MissionParameter_value_location;
    asn.value.u.location = new ASN1T_Location();
    location_.Send( *asn.value.u.location );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Location::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Location::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.location;
}
