// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentKnowledgeParameter.h"

#include "MIL_ParameterType_AgentKnowledge.h"
#include "Network/NET_ASN_Tools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_AgentKnowledgeParameter::MIL_AgentKnowledgeParameter( DEC_Knowledge_Agent* pKnowledge )
: pKnowledgeAgent_( pKnowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentKnowledgeParameter::MIL_AgentKnowledgeParameter( const ASN1T_UnitKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver )
: pKnowledgeAgent_( NET_ASN_Tools::ReadAgentKnowledge( asn, resolver ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentKnowledgeParameter::~MIL_AgentKnowledgeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentKnowledgeParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_AgentKnowledge*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter::ToAgentKnowledge
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentKnowledgeParameter::ToAgentKnowledge( ASN1T_UnitKnowledge& asn ) const
{
    asn = pKnowledgeAgent_->GetID();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter::ToAgentKnowledge
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentKnowledgeParameter::ToAgentKnowledge( DEC_Knowledge_Agent*& value ) const
{
    value = pKnowledgeAgent_;
    return true;
}
