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
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_LogMaintenancePrioritiesParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_LogMaintenancePrioritiesParameter::MIL_LogMaintenancePrioritiesParameter( const sword::LogMaintenancePriorities & asn )
{
    priorities_.reserve( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        sword::EquipmentType nCompTypeMosID = asn.elem(i);
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
// Name: MIL_LogMedicalPrioritiesParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_LogMaintenancePrioritiesParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eMaintenancePriorities;
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

// -----------------------------------------------------------------------------
// Name: MIL_LogMaintenancePrioritiesParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_LogMaintenancePrioritiesParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    for( std::size_t i = 0; i < priorities_.size(); ++i )
        elem.mutable_logmaintenancepriorities()->add_elem()->set_id( priorities_[ i ]->GetMosID().id() );
    return true;
}
