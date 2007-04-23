// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_EquipmentType.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_EquipmentType constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_EquipmentType::MissionParameter_EquipmentType( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , equipmentType_      ( asn.value.u.typeEquipement )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_EquipmentType destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_EquipmentType::~MissionParameter_EquipmentType()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_EquipmentType::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_EquipmentType::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value             = bNullValue_;
    asn.value.t                = T_MissionParameter_value_typeEquipement;
    asn.value.u.typeEquipement = equipmentType_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_EquipmentType::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_EquipmentType::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}
