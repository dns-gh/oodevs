// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_IndirectFire.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_IndirectFire constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_IndirectFire::MissionParameter_IndirectFire( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , fire_               ( asn.value().tirindirect() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IndirectFire destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_IndirectFire::~MissionParameter_IndirectFire()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_IndirectFire::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_IndirectFire::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
//    asn.value.t             = T_MissionParameter_value_tirIndirect;
    asn.mutable_value()->set_tirindirect( fire_ );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IndirectFire::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_IndirectFire::Delete( Common::MsgMissionParameter& /*asn*/ ) const
{
    // NOTHING
}
