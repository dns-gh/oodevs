// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_ObjectKnowledge.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledge constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledge::MissionParameter_ObjectKnowledge( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , objectKnowledge_    ( asn.value().objectknowledge().id() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledge destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledge::~MissionParameter_ObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledge::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectKnowledge::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_objectknowledge()->set_id( objectKnowledge_ );
}
