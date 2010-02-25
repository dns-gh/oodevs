// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Point.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Point constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Point::MissionParameter_Point( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , point_( asn.value().point().location() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Point destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Point::~MissionParameter_Point()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Point::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Point::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    point_.Send( *asn.mutable_value()->mutable_point()->mutable_location() );
}
