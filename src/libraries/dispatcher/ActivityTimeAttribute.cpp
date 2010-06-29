// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ActivityTimeAttribute.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ActivityTimeAttribute::ActivityTimeAttribute( const Common::MsgObjectAttributes& asnMsg )
    : nActivityTime_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ActivityTimeAttribute::~ActivityTimeAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if( asnMsg.has_activity_time()  )
        nActivityTime_ = asnMsg.activity_time().value();
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_activity_time()->set_value( nActivityTime_ );
}
