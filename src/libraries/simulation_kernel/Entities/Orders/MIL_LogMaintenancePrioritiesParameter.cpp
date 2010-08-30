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
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_LogMaintenancePrioritiesParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_LogMaintenancePrioritiesParameter::MIL_LogMaintenancePrioritiesParameter( const Common::MsgLogMaintenancePriorities & asn )
{
    priorities_.reserve( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        Common::EquipmentType nCompTypeMosID = asn.elem(i);
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
bool MIL_LogMaintenancePrioritiesParameter::ToMaintenancePriorities( Common::MsgLogMaintenancePriorities& asn ) const
{
    unsigned int size = priorities_.size();
    if( size )
        for( unsigned int i = 0; i < size; ++i )
            asn.add_elem()->set_id( priorities_[ i ]->GetMosID().id() );
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
