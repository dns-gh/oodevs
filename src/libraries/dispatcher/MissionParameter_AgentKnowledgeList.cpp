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
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentKnowledgeList::MissionParameter_AgentKnowledgeList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( int i = 0; i < asn.value().unitknowledgelist().elem_size(); ++i)
        agentKnowledges_.push_back( asn.value().unitknowledgelist().elem( i ).oid() );    
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
void MissionParameter_AgentKnowledgeList::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value ( bNullValue_ );
    for (std::vector< int >::const_iterator iter(agentKnowledges_.begin()); iter != agentKnowledges_.end(); ++iter)
        asn.mutable_value()->mutable_unitknowledgelist()->add_elem()->set_oid( *iter );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentKnowledgeList::Delete( Common::MsgMissionParameter& asn ) const
{
    delete asn.mutable_value()->mutable_unitknowledgelist();
}
