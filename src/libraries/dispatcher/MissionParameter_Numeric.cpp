// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Numeric.h"
#include "ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Numeric constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Numeric::MissionParameter_Numeric( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , value_              ( asn.value().areal() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Numeric destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Numeric::~MissionParameter_Numeric()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Numeric::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Numeric::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( bNullValue_ );
    asn.mutable_value()->set_areal( value_ );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Numeric::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Numeric::Delete( Common::MsgMissionParameter& /*asn*/ ) const
{
    // NOTHING
}
