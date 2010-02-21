// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Line.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Line constructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_Line::MissionParameter_Line( const Common::MsgMissionParameter& asn )
    : MissionParameter_ABC( asn )
    , location_( asn.value().line().location() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Line destructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_Line::~MissionParameter_Line()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Line::Send
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_Line::Send( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value   ( bNullValue_ );
//    asn.mutable_value()->mutable_line() = new MsgLine();
    location_.Send( *asn.mutable_value()->mutable_line()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Line::Delete
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_Line::Delete( Common::MsgMissionParameter& asn ) const
{
    delete asn.mutable_value()->mutable_line();
}
