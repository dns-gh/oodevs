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
#include "simulation_orders/MIL_ParameterType_EquipmentType.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"

// -----------------------------------------------------------------------------
// Name: MIL_EquipmentTypeParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_EquipmentTypeParameter::MIL_EquipmentTypeParameter( const ASN1T_EquipmentType & asn )
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
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_EquipmentTypeParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_EquipmentType*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_EquipmentTypeParameter::ToEquipmentType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_EquipmentTypeParameter::ToEquipmentType( ASN1T_EquipmentType& asn ) const
{
    if( !pType_ )
        return false;
    asn = pType_->GetMosID();
    return true;
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
