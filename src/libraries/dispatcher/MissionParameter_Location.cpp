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
#include "protocol/ClientPublisher_ABC.h"
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

// -----------------------------------------------------------------------------
// Name: MissionParameter_Location::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Location::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    location_.Send( *asn.mutable_value()->mutable_location() );
}
