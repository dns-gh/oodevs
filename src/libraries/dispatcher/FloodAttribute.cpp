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
FloodAttribute::FloodAttribute( const sword::ObjectAttributes& msg )
    : depth_      ( 0 )
    , refDist_    ( 0 )
{
    Update( msg );
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
void FloodAttribute::Update( const sword::ObjectAttributes& msg )
{
    if( msg.has_flood() )
    {
        depth_ = msg.flood().depth();
        refDist_ = msg.flood().reference_distance();
    }
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Send
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
void FloodAttribute::Send( sword::ObjectAttributes& msg ) const
{
    msg.mutable_flood()->set_depth( depth_ );
    msg.mutable_flood()->set_reference_distance( refDist_ );
}
