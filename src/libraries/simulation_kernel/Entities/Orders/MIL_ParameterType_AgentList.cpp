// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ParameterType_AgentList.h"

#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentList constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentList::MIL_ParameterType_AgentList()
    : MIL_ParameterType_ABC( "AgentList" )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentList destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentList::~MIL_ParameterType_AgentList()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentList::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_AgentList::Copy( const ASN1T_MissionParameter& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( from.null_value || from.value.t != T_MissionParameter_value_listAgent ) 
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    // Check dest
    if( !DEC_Tools::CheckTypeListePions( to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );

    if( !NET_ASN_Tools::CopyAgentList( *from.value.u.listAgent, to ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission_parameters );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentList::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AgentList::Copy( const DIA_Variable_ABC& from, DIA_Variable_ABC& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListePions( from ) )
        return false;

    // Check dest
    if( !DEC_Tools::CheckTypeListePions( to ) )
        return false;

    if( !NET_ASN_Tools::CopyAgentList( from, to ) )
        return false;
    
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentList::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AgentList::Copy( const DIA_Variable_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !DEC_Tools::CheckTypeListePions( from ) )
        return false;

    to.null_value        = false;
    to.value.t           = T_MissionParameter_value_listAgent;
    to.value.u.listAgent = new ASN1T_ListAgent();
    
    if( !NET_ASN_Tools::CopyAgentList( from, *to.value.u.listAgent ) )
        return false;

    return true;    
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentList::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_AgentList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_listAgent );
    assert( to.value.u.listAgent );
    NET_ASN_Tools::Delete( *to.value.u.listAgent );
    delete to.value.u.listAgent;
}
