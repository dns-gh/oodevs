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
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "Network/NET_AsnException.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_AgentKnowledgeParameter::MIL_AgentKnowledgeParameter( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
    : pKnowledgeAgent_( pKnowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentKnowledgeParameter::MIL_AgentKnowledgeParameter( const sword::UnitKnowledgeId& asn, const DEC_KnowledgeResolver_ABC& resolver )
    : pKnowledgeAgent_( NET_ASN_Tools::ReadAgentKnowledge( asn, resolver ) )
{
    if( !pKnowledgeAgent_ )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck_ErrorCode_error_invalid_mission_parameters );
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
bool MIL_AgentKnowledgeParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eAgentKnowledge
        || type == MIL_ParameterType_ABC::eLocationComposite;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter::ToAgentKnowledge
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentKnowledgeParameter::ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& value ) const
{
    value = pKnowledgeAgent_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_AgentKnowledgeParameter::ToElement( sword::MsgMissionParameter_Value& elem ) const
{
    elem.mutable_agentknowledge()->set_id( pKnowledgeAgent_->GetID() );
    return true;
}
