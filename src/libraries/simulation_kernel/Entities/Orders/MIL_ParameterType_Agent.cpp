// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_Agent.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Agent constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Agent::MIL_ParameterType_Agent()
    : MIL_ParameterType_ABC( "Agent" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Agent destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_Agent::~MIL_ParameterType_Agent()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Agent::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_Agent::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_unit ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypePion( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyAgent( from.value.u.unit, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Agent::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Agent::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypePion( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypePion( to ) )
        return false;

    if( !NET_ASN_Tools::CopyAgent( from, to ) )
        return false;
    
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Agent::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_Agent::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypePion( from ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_unit;
    
    if( !NET_ASN_Tools::CopyAgent( from, to.value.u.unit ) )
        return false;

    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_Agent::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_Agent::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_unit );
}
