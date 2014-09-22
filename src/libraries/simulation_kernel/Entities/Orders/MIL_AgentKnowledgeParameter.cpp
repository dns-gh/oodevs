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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/NET_AsnException.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentKnowledgeParameter )

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_AgentKnowledgeParameter::MIL_AgentKnowledgeParameter()
{
    // NOTHING
}

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
MIL_AgentKnowledgeParameter::MIL_AgentKnowledgeParameter( const sword::Id& asn, const DEC_KnowledgeResolver_ABC& resolver,
                                                          const MIL_EntityManager_ABC& entityManager )
{
    MIL_AgentPion* pAgent = entityManager.FindAgentPion( asn.id() );
    if( !pAgent )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
    pKnowledgeAgent_ = resolver.ResolveKnowledgeAgent( *pAgent );
    if( !pKnowledgeAgent_ )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
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
    return type == MIL_ParameterType_ABC::eAgent
        || type == MIL_ParameterType_ABC::eAgentKnowledge
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
bool MIL_AgentKnowledgeParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.mutable_agentknowledge()->set_id( pKnowledgeAgent_->GetAgentKnown().GetID() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter::load
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void MIL_AgentKnowledgeParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> pKnowledgeAgent_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeParameter::save
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void MIL_AgentKnowledgeParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << pKnowledgeAgent_;
}
