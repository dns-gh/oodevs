// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Enumeration.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Enumeration constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Enumeration::MissionParameter_Enumeration( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , enumeration_        ( asn.value().enumeration() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Enumeration destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Enumeration::~MissionParameter_Enumeration()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Enumeration::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Enumeration::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->set_enumeration( enumeration_ );
}
