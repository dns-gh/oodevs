// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_AgentKnowledgeList.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList constructor
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentKnowledgeList::MIL_ParameterType_AgentKnowledgeList()
    : MIL_ParameterType_ABC( "AgentKnowledgeList" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList destructor
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentKnowledgeList::~MIL_ParameterType_AgentKnowledgeList()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList::Copy
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
void MIL_ParameterType_AgentKnowledgeList::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( from.value.t != T_MissionParameter_value_listKnowledgeAgent ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeListeConnaissancesAgent( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyAgentKnowledgeList( *from.value.u.listKnowledgeAgent, to, knowledgeResolver ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList::Copy
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AgentKnowledgeList::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeConnaissancesAgent( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeListeConnaissancesObjet( to ) )
        return false;

    NET_ASN_Tools::CopyAgentKnowledgeList( from, to );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList::Copy
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AgentKnowledgeList::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListeConnaissancesAgent( from ) )
        return false;

    to.null_value                 = false;
    to.value.t                    = T_MissionParameter_value_listKnowledgeAgent;
    to.value.u.listKnowledgeAgent = new ASN1T_ListKnowledgeAgent();

    if( !NET_ASN_Tools::CopyAgentKnowledgeList( from, *to.value.u.listKnowledgeAgent, knowledgeResolver ) )
        return false;

    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList::CleanAfterSerialization
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
void MIL_ParameterType_AgentKnowledgeList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_listKnowledgeAgent );
    assert( to.value.u.listKnowledgeAgent );
    NET_ASN_Tools::Delete( *to.value.u.listKnowledgeAgent );
    delete to.value.u.listKnowledgeAgent;
}
