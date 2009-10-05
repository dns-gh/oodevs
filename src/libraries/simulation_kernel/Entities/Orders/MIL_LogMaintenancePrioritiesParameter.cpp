// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LogMaintenancePrioritiesParameter.h"
#include "simulation_orders/MIL_ParameterType_MaintenancePriorities.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"

// -----------------------------------------------------------------------------
// Name: MIL_LogMaintenancePrioritiesParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_LogMaintenancePrioritiesParameter::MIL_LogMaintenancePrioritiesParameter( const ASN1T_LogMaintenancePriorities & asn )
{
    priorities_.reserve( asn.n );
    for( unsigned int i = 0; i < asn.n; ++i )
    {
        unsigned int nCompTypeMosID = asn.elem[i];
        const PHY_ComposanteTypePion* pType = PHY_ComposanteTypePion::Find( nCompTypeMosID );
        if( !pType )
        {
            priorities_.clear();
            break;
        }
        priorities_.push_back( pType );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMaintenancePrioritiesParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_LogMaintenancePrioritiesParameter::~MIL_LogMaintenancePrioritiesParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMaintenancePrioritiesParameter::IsOfType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_LogMaintenancePrioritiesParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_MaintenancePriorities*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMaintenancePrioritiesParameter::ToMaintenancePriorities
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_LogMaintenancePrioritiesParameter::ToMaintenancePriorities( ASN1T_LogMaintenancePriorities& asn ) const
{
    unsigned int size = priorities_.size();
    asn.n = size;
    if( size )
    {
        asn.elem = new ASN1T_EquipmentType[ size ];
        for( unsigned int i = 0; i < size; ++i )
            asn.elem[ i ] = priorities_[ i ]->GetMosID();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMaintenancePrioritiesParameter::ToMaintenancePriorities
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_LogMaintenancePrioritiesParameter::ToMaintenancePriorities( T_MaintenancePriorityVector& value ) const
{
    value = priorities_;
    return true;
}
