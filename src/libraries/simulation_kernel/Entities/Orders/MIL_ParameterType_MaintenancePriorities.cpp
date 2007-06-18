// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_MaintenancePriorities.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MaintenancePriorities constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_MaintenancePriorities::MIL_ParameterType_MaintenancePriorities()
    : MIL_ParameterType_ABC( "MaintenancePriorities" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MaintenancePriorities destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_MaintenancePriorities::~MIL_ParameterType_MaintenancePriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MaintenancePriorities::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_MaintenancePriorities::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_logMaintenancePriorities ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeMaintenancePriorities( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyMaintenancePriorities( *from.value.u.logMaintenancePriorities, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MaintenancePriorities::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_MaintenancePriorities::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeMaintenancePriorities( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeMaintenancePriorities( to ) )
        return false;

    NET_ASN_Tools::CopyMaintenancePriorities( from, to );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MaintenancePriorities::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_MaintenancePriorities::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeMaintenancePriorities( from ) )
        return false;

    to.null_value                   = false;
    to.value.t                      = T_MissionParameter_value_logMaintenancePriorities;
    to.value.u.logMaintenancePriorities = new ASN1T_LogMaintenancePriorities();
    
    if( !NET_ASN_Tools::CopyMaintenancePriorities( from, *to.value.u.logMaintenancePriorities ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_MaintenancePriorities::CleanAfterSerialization
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void MIL_ParameterType_MaintenancePriorities::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_logMaintenancePriorities );
    assert( to.value.u.logMaintenancePriorities );
    NET_ASN_Tools::Delete( *to.value.u.logMaintenancePriorities );
    delete to.value.u.logMaintenancePriorities;
}
