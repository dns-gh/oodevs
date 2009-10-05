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
#include "Network/NET_ASN_Tools.h"
#include "MIL_MissionParameter_ABC.h"

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
bool MIL_ParameterType_MaintenancePriorities::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.value.t                          = T_MissionParameter_value_logMaintenancePriorities;
    to.value.u.logMaintenancePriorities = new ASN1T_LogMaintenancePriorities();
    to.null_value                       = !from.ToMaintenancePriorities( *to.value.u.logMaintenancePriorities );
    
    return !to.null_value || bIsOptional;
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
