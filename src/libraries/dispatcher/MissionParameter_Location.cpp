// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Location.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Location constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Location::MissionParameter_Location( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , location_           ( asn.value().location() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Location destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Location::~MissionParameter_Location()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Location::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Location::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value           ( bNullValue_ );
    //asn.mutable_value()->mutable_location() = new MsgLocation();
    location_.Send( *asn.mutable_value()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Location::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Location::Delete( Common::MsgMissionParameter& asn ) const
{
    delete asn.mutable_value()->mutable_location();
}
