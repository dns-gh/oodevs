// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_Objective.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Objective constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Objective::MIL_ParameterType_Objective()
    : MIL_ParameterType_ABC( "Objective" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Objective destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Objective::~MIL_ParameterType_Objective()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Objective::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_Objective::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_missionObjective ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeObjectif( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    NET_ASN_Tools::CopyObjective( *from.value.u.missionObjective, to );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Objective::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Objective::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeObjectif( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeObjectif( to ) )
        return false;

    NET_ASN_Tools::CopyObjective( from, to );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Objective::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Objective::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeObjectif( from ) )
        return false;

    to.null_value               = false;
    to.value.t                  = T_MissionParameter_value_missionObjective;
    to.value.u.missionObjective = new ASN1T_MissionObjective();
    
    return NET_ASN_Tools::CopyObjective( from, *to.value.u.missionObjective );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Objective::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_Objective::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_missionObjective );
    assert( to.value.u.missionObjective );
    NET_ASN_Tools::Delete( *to.value.u.missionObjective );
    delete to.value.u.missionObjective;    
}
