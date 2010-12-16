// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_EquipmentTypeParameter.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"

// -----------------------------------------------------------------------------
// Name: MIL_EquipmentTypeParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_EquipmentTypeParameter::MIL_EquipmentTypeParameter( const sword::EquipmentType & asn )
    : pType_( PHY_ComposanteTypePion::Find( asn ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_EquipmentTypeParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_EquipmentTypeParameter::MIL_EquipmentTypeParameter( const PHY_ComposanteTypePion* pEquipmentType )
    : pType_( pEquipmentType )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_EquipmentTypeParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_EquipmentTypeParameter::~MIL_EquipmentTypeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_EquipmentTypeParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_EquipmentTypeParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eEquipmentType;
}

// -----------------------------------------------------------------------------
// Name: MIL_EquipmentTypeParameter::ToEquipmentType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_EquipmentTypeParameter::ToEquipmentType( const PHY_ComposanteTypePion*& value ) const
{
    value = pType_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_EquipmentTypeParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_EquipmentTypeParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    if( !pType_ )
        return false;
    *elem.mutable_equipmenttype() = pType_->GetMosID();
    return true;
}
