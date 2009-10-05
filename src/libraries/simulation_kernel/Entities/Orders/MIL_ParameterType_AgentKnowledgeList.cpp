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

#include "Network/NET_ASN_Tools.h"
#include "MIL_MissionParameter_ABC.h"

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
bool MIL_ParameterType_AgentKnowledgeList::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.null_value                 = false;
    to.value.t                    = T_MissionParameter_value_unitKnowledgeList;
    to.value.u.unitKnowledgeList = new ASN1T_UnitKnowledgeList();

    return from.ToAgentKnowledgeList( *to.value.u.unitKnowledgeList );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList::CleanAfterSerialization
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
void MIL_ParameterType_AgentKnowledgeList::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_unitKnowledgeList );
    assert( to.value.u.unitKnowledgeList );
    NET_ASN_Tools::Delete( *to.value.u.unitKnowledgeList );
    delete to.value.u.unitKnowledgeList;
}
