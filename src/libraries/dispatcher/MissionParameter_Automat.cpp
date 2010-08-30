// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Automat.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Automat constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Automat::MissionParameter_Automat( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , automat_            ( asn.value().automat().id() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Automat destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Automat::~MissionParameter_Automat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Automat::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Automat::Send( Common::MsgMissionParameter& asn ) const
{
    MissionParameter_ABC::Send( asn );
    asn.mutable_value()->mutable_automat()->set_id( automat_ );
}
