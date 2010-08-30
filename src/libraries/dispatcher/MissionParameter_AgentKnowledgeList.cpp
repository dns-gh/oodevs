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
    for( int i = 0; i < asn.value().unitknowledgelist().elem_size(); ++i )
        agentKnowledges_.push_back( asn.value().unitknowledgelist().elem( i ).id() );    
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
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_unitknowledgelist();
    for( std::vector< int >::const_iterator it = agentKnowledges_.begin(); it != agentKnowledges_.end(); ++it )
        asn.mutable_value()->mutable_unitknowledgelist()->add_elem()->set_id( *it );
}
