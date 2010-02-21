// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Direction.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Direction constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Direction::MissionParameter_Direction( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , direction_          ( asn.value().heading().heading() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Direction destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Direction::~MissionParameter_Direction()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Direction::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Direction::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
//    asn.value.t         = T_MissionParameter_value_heading;
    asn.mutable_value()->mutable_heading()->set_heading( direction_ );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Direction::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Direction::Delete( Common::MsgMissionParameter& /*asn*/ ) const
{
    // NOTHING
}
