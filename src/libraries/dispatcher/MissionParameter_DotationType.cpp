// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_DotationType.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_DotationType constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_DotationType::MissionParameter_DotationType( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , dotationType_       ( asn.value().resourcetype().id() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_DotationType destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_DotationType::~MissionParameter_DotationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_DotationType::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_DotationType::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_resourcetype()->set_id( dotationType_ );
}
