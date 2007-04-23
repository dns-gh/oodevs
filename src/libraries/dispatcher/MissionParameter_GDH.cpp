// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_GDH.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_GDH constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GDH::MissionParameter_GDH( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , datation_           ( asn.value.u.gDH->datation )
    , qualificatif_       ( asn.value.u.gDH->qualificatif )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GDH destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GDH::~MissionParameter_GDH()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_GDH::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GDH::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value                  = bNullValue_;
    asn.value.t                     = T_MissionParameter_value_gDH;
    asn.value.u.gDH                 = new ASN1T_GDH();
    asn.value.u.gDH->datation       = datation_;
    asn.value.u.gDH->qualificatif   = qualificatif_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GDH::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GDH::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.gDH;
}
