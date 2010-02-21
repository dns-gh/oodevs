// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Path.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Path constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Path::MissionParameter_Path( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , path_               ( asn.value().path().location() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Path destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Path::~MissionParameter_Path()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Path::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Path::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value     ( bNullValue_ );
    //asn.value.t            = T_MissionParameter_value_path;
    asn.mutable_value()->mutable_path();
    path_.Send( *asn.mutable_value()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Path::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Path::Delete( Common::MsgMissionParameter& asn ) const
{
    delete asn.mutable_value()->mutable_path();
}
