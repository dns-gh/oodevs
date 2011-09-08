// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "TrafficabilityAttribute.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: CMA 2006-09-26
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute( const sword::ObjectAttributes& asnMsg )
    : max_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute destructor
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
TrafficabilityAttribute::~TrafficabilityAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::Update
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_trafficability() )
        max_ = asnMsg.trafficability().value();
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::Send
// Created: CMA 2011-09-06
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_trafficability()->set_value( max_ );
}
