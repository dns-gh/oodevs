// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_AgentList.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentList::MissionParameter_AgentList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for (int i = 0; i < asn.value().unitlist().elem_size(); ++i)
        agents_.push_back( asn.value().unitlist().elem( i ).oid() );
//    std::copy( asn.value().unitlist().elem().begin(), asn.value().unitlist().elem().end(), std::back_inserter( agents_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentList::~MissionParameter_AgentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentList::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value ( bNullValue_ );
    for (std::vector< int >::const_iterator iter(agents_.begin()); iter != agents_.end(); ++iter )
        asn.mutable_value()->mutable_unitlist()->add_elem()->set_oid( *iter );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentList::Delete( Common::MsgMissionParameter& asn ) const
{
    delete asn.mutable_value()->mutable_unitlist();
}
