// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LogMedicalPrioritiesParameter.h"
#include "MIL_ParameterType_MedicalPriorities.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_LogMedicalPrioritiesParameter::MIL_LogMedicalPrioritiesParameter( const ASN1T_LogMedicalPriorities & asn )
{
    priorities_.reserve( asn.n );
    for( unsigned int i = 0; i < asn.n; ++i )
    {
        ASN1T_EnumHumanWound nWoundID = asn.elem[i];
        const PHY_HumanWound* pWound = PHY_HumanWound::Find( nWoundID );
        if( !pWound )
        {
            priorities_.clear();
            break;
        }
        priorities_.push_back( pWound );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_LogMedicalPrioritiesParameter::~MIL_LogMedicalPrioritiesParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter::IsOfType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_LogMedicalPrioritiesParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_MedicalPriorities*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter::ToMedicalPriorities
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_LogMedicalPrioritiesParameter::ToMedicalPriorities( ASN1T_LogMedicalPriorities& asn ) const
{
    unsigned int size = priorities_.size();
    asn.n = size;
    if( size )
    {
        asn.elem = new ASN1T_EnumHumanWound[ size ];
        for( unsigned int i = 0; i < size; ++i )
            asn.elem[ i ] = priorities_[ i ]->GetAsnID();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LogMedicalPrioritiesParameter::ToMedicalPriorities
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_LogMedicalPrioritiesParameter::ToMedicalPriorities( T_MedicalPriorityVector& value ) const
{
    value = priorities_;
    return true;
}
