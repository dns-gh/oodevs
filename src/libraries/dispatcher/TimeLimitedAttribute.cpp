// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "TimeLimitedAttribute.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
TimeLimitedAttribute::TimeLimitedAttribute( const sword::ObjectAttributes& asnMsg )
    : lifeTime_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
TimeLimitedAttribute::~TimeLimitedAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_life_time() )
        lifeTime_ = asnMsg.life_time().value();
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void TimeLimitedAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_life_time()->set_value( lifeTime_ );
}
