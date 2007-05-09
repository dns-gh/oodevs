// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_NatureAtlas.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_NatureAtlas constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_NatureAtlas::MissionParameter_NatureAtlas( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , natureAtlas_        ( asn.value.u.natureAtlas->numbits, asn.value.u.natureAtlas->data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_NatureAtlas destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_NatureAtlas::~MissionParameter_NatureAtlas()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_NatureAtlas::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_NatureAtlas::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value          = bNullValue_;
    asn.value.t             = T_MissionParameter_value_natureAtlas;
    asn.value.u.natureAtlas = new ASN1T_NatureAtlas( natureAtlas_.numbits, natureAtlas_.data );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_NatureAtlas::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_NatureAtlas::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.natureAtlas;
}
