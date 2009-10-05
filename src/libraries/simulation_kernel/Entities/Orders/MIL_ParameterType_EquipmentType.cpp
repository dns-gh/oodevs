// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_EquipmentType.h"

#include "MIL_MissionParameter_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_EquipmentType constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_EquipmentType::MIL_ParameterType_EquipmentType()
: MIL_ParameterType_ABC( "EquipmentType" )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_EquipmentType destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_EquipmentType::~MIL_ParameterType_EquipmentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_EquipmentType::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_EquipmentType::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_equipmentType;
    
    return from.ToEquipmentType( to.value.u.equipmentType );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_EquipmentType::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_EquipmentType::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_equipmentType );
}
