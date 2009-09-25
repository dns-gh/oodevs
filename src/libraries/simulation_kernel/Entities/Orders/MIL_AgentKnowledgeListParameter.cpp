// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentKnowledgeListParameter.h"

#include "MIL_ParameterType_AgentKnowledgeList.h"
#include "Network/NET_ASN_Tools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeListParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentKnowledgeListParameter::MIL_AgentKnowledgeListParameter( const ASN1T_UnitKnowledgeList& asn, const DEC_KnowledgeResolver_ABC& resolver )
{
    knowledgeAgentList_.reserve( asn.n );
    for( unsigned int i = 0; i < asn.n; ++i )
    {
        DEC_Knowledge_Agent* pKnowledgeAgent = NET_ASN_Tools::ReadAgentKnowledge( asn.elem[i], resolver );
        if( !pKnowledgeAgent )
            throw std::runtime_error( "Agent Knowledge does not exist" );
        knowledgeAgentList_.push_back( pKnowledgeAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeListParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_AgentKnowledgeListParameter::MIL_AgentKnowledgeListParameter( const std::vector< DEC_Knowledge_Agent* >& knowledgeAgentList )
: knowledgeAgentList_( knowledgeAgentList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeListParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentKnowledgeListParameter::~MIL_AgentKnowledgeListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeListParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentKnowledgeListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_AgentKnowledgeList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeListParameter::ToAgentKnowledgeList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentKnowledgeListParameter::ToAgentKnowledgeList( ASN1T_UnitKnowledgeList& asn ) const
{
    unsigned int size = knowledgeAgentList_.size();
    asn.n = size;
    if( size != 0 )
    {    
        ASN1T_OID* pOID = new ASN1T_OID[ size ]; //$$$ RAM
        asn.elem = pOID;

        for( unsigned int i = 0; i < size; ++i )
            pOID[i] = knowledgeAgentList_[i]->GetID();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentKnowledgeListParameter::ToAgentKnowledgeList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentKnowledgeListParameter::ToAgentKnowledgeList( std::vector< DEC_Knowledge_Agent* >& value ) const
{
    value.resize( knowledgeAgentList_.size() );
    for( unsigned int i = 0; i < knowledgeAgentList_.size(); ++i )
        value[i] = knowledgeAgentList_[i];
    return true;
}
