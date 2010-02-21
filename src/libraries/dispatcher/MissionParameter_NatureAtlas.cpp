// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_NatureAtlas.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_NatureAtlas constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_NatureAtlas::MissionParameter_NatureAtlas( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , natureAtlas_( asn.value().atlasnature() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_NatureAtlas destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_NatureAtlas::~MissionParameter_NatureAtlas()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_NatureAtlas::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_NatureAtlas::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
    asn.mutable_value()->mutable_atlasnature()->set_nature( natureAtlas_.nature() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_NatureAtlas::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_NatureAtlas::Delete( Common::MsgMissionParameter& asn ) const
{
    delete asn.mutable_value()->mutable_atlasnature();
}
