// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_AgentKnowledge.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentKnowledge::MIL_ParameterType_AgentKnowledge()
    : MIL_ParameterType_ABC( "AgentKnowledge" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentKnowledge::~MIL_ParameterType_AgentKnowledge()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_AgentKnowledge::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_knowledgeAgent ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeConnaissanceAgent( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyAgentKnowledge( from.value.u.knowledgeAgent, to, knowledgeResolver ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AgentKnowledge::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeConnaissanceAgent( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeConnaissanceAgent( to ) )
        return false;

    NET_ASN_Tools::CopyAgentKnowledge( from, to );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AgentKnowledge::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeConnaissanceAgent( from ) )
        return false;

    to.null_value = false;
    to.value.t    = T_MissionParameter_value_knowledgeAgent;
    
    if( !NET_ASN_Tools::CopyAgentKnowledge( from, to.value.u.knowledgeAgent, knowledgeResolver ) )
        return false;

    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_AgentKnowledge::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_knowledgeObject );
}
