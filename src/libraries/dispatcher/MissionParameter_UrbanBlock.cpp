// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_UrbanBlock.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_UrbanBlock constructor
// Created: SLG 2009-11-06
// -----------------------------------------------------------------------------
MissionParameter_UrbanBlock::MissionParameter_UrbanBlock( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , block_              ( asn.value().urbanblock().oid() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_UrbanBlock destructor
// Created: SLG 2009-11-06
// -----------------------------------------------------------------------------
MissionParameter_UrbanBlock::~MissionParameter_UrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_UrbanBlock::Send
// Created: SLG 2009-11-06
// -----------------------------------------------------------------------------
void MissionParameter_UrbanBlock::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
    asn.mutable_value()->mutable_urbanblock()->set_oid( block_ ); 
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_UrbanBlock::Delete
// Created: SLG 2009-11-06
// -----------------------------------------------------------------------------
void MissionParameter_UrbanBlock::Delete( Common::MsgMissionParameter& /*asn*/ ) const
{
    // NOTHING
}
