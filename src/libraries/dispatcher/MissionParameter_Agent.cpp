// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Agent.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Agent constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Agent::MissionParameter_Agent( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , agent_              ( asn.value().unit().oid() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Agent destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Agent::~MissionParameter_Agent()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Agent::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Agent::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
//    asn.value.t       = T_MissionParameter_value_unit;
    asn.mutable_value()->mutable_unit()->set_oid( agent_ );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Agent::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Agent::Delete( Common::MsgMissionParameter& /*asn*/ ) const
{
    // NOTHING
}
