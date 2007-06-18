// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_MedicalPriorities.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_MedicalPriorities::MIL_ParameterType_MedicalPriorities()
    : MIL_ParameterType_ABC( "MedicalPriorities" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_MedicalPriorities::~MIL_ParameterType_MedicalPriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_MedicalPriorities::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_logMedicalPriorities ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeSantePriorites( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyMedicalPriorities( *from.value.u.logMedicalPriorities, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_MedicalPriorities::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeSantePriorites( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeSantePriorites( to ) )
        return false;

    NET_ASN_Tools::CopyMedicalPriorities( from, to );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_MedicalPriorities::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeSantePriorites( from ) )
        return false;

    to.null_value             = false;
    to.value.t                = T_MissionParameter_value_logMedicalPriorities;
    to.value.u.logMedicalPriorities = new ASN1T_LogMedicalPriorities();
    
    if( !NET_ASN_Tools::CopyMedicalPriorities( from, *to.value.u.logMedicalPriorities ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MedicalPriorities::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_MedicalPriorities::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_logMedicalPriorities );
    assert( to.value.u.logMedicalPriorities );
    NET_ASN_Tools::Delete( *to.value.u.logMedicalPriorities );
    delete to.value.u.logMedicalPriorities;    
}
