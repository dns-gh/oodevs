// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "FloodAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( const sword::ObjectAttributes& asnMsg )
    : depth_  ( 0 )
    , refDist_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute destructor
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
FloodAttribute::~FloodAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Update
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
void FloodAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_flood() )
    {
        depth_ = asnMsg.flood().depth();
        refDist_ = asnMsg.flood().reference_distance();
    }
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Send
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
void FloodAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_flood()->set_depth( depth_ );
    asnMsg.mutable_flood()->set_reference_distance( refDist_ );
}
