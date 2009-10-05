// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_AgentKnowledge.h"
#include "MIL_MissionParameter_ABC.h"

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
bool MIL_ParameterType_AgentKnowledge::Copy( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.value.t    = T_MissionParameter_value_unitKnowledge;
    to.null_value = !from.ToAgentKnowledge( to.value.u.unitKnowledge );
    
    return !to.null_value || bIsOptional;
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_ParameterType_AgentKnowledge::CleanAfterSerialization( ASN1T_MissionParameter& to ) const
{
    assert( to.value.t == T_MissionParameter_value_unitKnowledge );
}
