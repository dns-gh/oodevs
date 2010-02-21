// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_AgentKnowledgeList.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList constructor
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentKnowledgeList::MIL_ParameterType_AgentKnowledgeList( const std::string& name )
    : MIL_ParameterType_ABC( name )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList destructor
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentKnowledgeList::~MIL_ParameterType_AgentKnowledgeList()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList::Copy
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AgentKnowledgeList::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.set_null_value( false );
    return from.ToAgentKnowledgeList( *to.mutable_value()->mutable_unitknowledgelist() );
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledgeList::CleanAfterSerialization
// Created: NLD 2006-11-AgentList
//-----------------------------------------------------------------------------
void MIL_ParameterType_AgentKnowledgeList::CleanAfterSerialization( Common::MsgMissionParameter& to ) const
{
    assert( to.mutable_value()->mutable_unitknowledgelist() );
    to.mutable_value()->clear_unitknowledgelist();
}
