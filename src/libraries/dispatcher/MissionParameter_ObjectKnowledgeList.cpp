// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_ObjectKnowledgeList.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledgeList::MissionParameter_ObjectKnowledgeList( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( int i = 0; i < asn.value().objectknowledgelist().elem_size(); ++i )
        objectKnowledges_.push_back( asn.value().objectknowledgelist().elem( i ).oid() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledgeList::~MissionParameter_ObjectKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectKnowledgeList::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value ( bNullValue_ );
    for( std::vector< int >::const_iterator it = objectKnowledges_.begin(); it != objectKnowledges_.end(); ++it )
        asn.mutable_value()->mutable_objectknowledgelist()->add_elem()->set_oid( *it );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectKnowledgeList::Delete( Common::MsgMissionParameter& asn ) const
{
    asn.mutable_value()->clear_objectknowledgelist();
}
