// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "UndergroundAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute( const sword::ObjectAttributes& asnMsg )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute destructor
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
UndergroundAttribute::~UndergroundAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::Update
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
void UndergroundAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_underground() )
        network_ = asnMsg.underground().network();
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::Send
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
void UndergroundAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_underground()->set_network( network_ );
}
