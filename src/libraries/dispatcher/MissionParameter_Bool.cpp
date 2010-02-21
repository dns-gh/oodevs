// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Bool.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Bool constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Bool::MissionParameter_Bool( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , bValue_             ( asn.value().abool() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Bool destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Bool::~MissionParameter_Bool()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Bool::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Bool::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value    ( bNullValue_ ); 
    //asn.value.t       = T_MissionParameter_value_aBool;
    asn.mutable_value()->set_abool ( bValue_ );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Bool::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Bool::Delete( Common::MsgMissionParameter& /*asn*/ ) const
{
    // NOTHING
}
