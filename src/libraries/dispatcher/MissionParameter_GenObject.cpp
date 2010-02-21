// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_GenObject.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObject constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GenObject::MissionParameter_GenObject( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , data_               ( asn.value().plannedwork() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObject destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GenObject::~MissionParameter_GenObject()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObject::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GenObject::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value               ( bNullValue_ ) ;
//    asn.mutable_value()->mutable_plannedwork() = new MsgPlannedWork();
    data_.Send( *asn.mutable_value()->mutable_plannedwork() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObject::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GenObject::Delete( Common::MsgMissionParameter& asn ) const
{
    delete asn.mutable_value()->mutable_plannedwork();
}
