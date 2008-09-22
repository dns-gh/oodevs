// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_AgentKnowledgeList.h"
#include "game_asn/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentKnowledgeList::MissionParameter_AgentKnowledgeList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    std::copy( asn.value.u.unitKnowledgeList->elem, asn.value.u.unitKnowledgeList->elem + asn.value.u.unitKnowledgeList->n
             , std::back_inserter( agentKnowledges_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentKnowledgeList::~MissionParameter_AgentKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentKnowledgeList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = bNullValue_;
    asn.value.t = T_MissionParameter_value_unitKnowledgeList;
    asn.value.u.unitKnowledgeList = new ASN1T_UnitKnowledgeList();
    asn.value.u.unitKnowledgeList->n = agentKnowledges_.size();
    asn.value.u.unitKnowledgeList->elem = agentKnowledges_.empty() ? 0 : (int*)&agentKnowledges_.front();
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentKnowledgeList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.unitKnowledgeList;
}
