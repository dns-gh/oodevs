// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Polygon.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Polygon constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Polygon::MissionParameter_Polygon( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , polygon_           ( asn.value().polygon().location() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Polygon destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Polygon::~MissionParameter_Polygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Polygon::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Polygon::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    polygon_.Send( *asn.mutable_value()->mutable_polygon()->mutable_location() );
}
