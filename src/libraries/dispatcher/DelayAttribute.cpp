// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "DelayAttribute.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DelayAttribute constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::DelayAttribute( const Common::MsgObjectAttributes& asnMsg )
    : nDelay_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute destructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::~DelayAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::Update
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if( asnMsg.has_effect_delay()  )
        nDelay_ = asnMsg.effect_delay().value();
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::Send
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_effect_delay()->set_value( nDelay_ );
}
