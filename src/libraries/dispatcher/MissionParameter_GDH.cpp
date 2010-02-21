// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_GDH.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_GDH constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GDH::MissionParameter_GDH( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , datation_           ( asn.value().datetime() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GDH destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GDH::~MissionParameter_GDH()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_GDH::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GDH::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
    asn.mutable_value()->mutable_datetime()->set_data( datation_.data() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GDH::Delete
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void MissionParameter_GDH::Delete( Common::MsgMissionParameter& ) const
{
    // NOTHING
}
