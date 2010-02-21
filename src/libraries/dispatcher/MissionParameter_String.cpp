// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_String.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;


// -----------------------------------------------------------------------------
// Name: MissionParameter_String constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_String::MissionParameter_String( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , value_              ( asn.value().acharstr() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_String destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_String::~MissionParameter_String()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_String::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_String::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
    asn.mutable_value()->set_acharstr( value_.c_str() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_String::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_String::Delete( Common::MsgMissionParameter& /*asn*/ ) const
{
    // NOTHING
}
