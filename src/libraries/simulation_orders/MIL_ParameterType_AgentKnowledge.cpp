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
#include "protocol/protocol.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentKnowledge::MIL_ParameterType_AgentKnowledge( const std::string& name )
    : MIL_ParameterType_ABC( name )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AgentKnowledge::~MIL_ParameterType_AgentKnowledge()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AgentKnowledge::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AgentKnowledge::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( !from.ToAgentKnowledge( *to.mutable_value()->Add()->mutable_agentknowledge() ) );
    if( to.null_value() )
        to.clear_value();
    return !to.null_value() || bIsOptional;
}
