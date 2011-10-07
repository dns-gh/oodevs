// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AltitudeModifierAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( const sword::ObjectAttributes& msg )
    : height_     ( 0 )
{
    Update( msg );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::~AltitudeModifierAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Update
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::Update( const sword::ObjectAttributes& msg )
{
    if( msg.has_altitude_modifier() )
        height_ = msg.altitude_modifier().height();
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Send
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::Send( sword::ObjectAttributes& msg ) const
{
    msg.mutable_altitude_modifier()->set_height( height_ );
}
