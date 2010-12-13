// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "InteractionHeightAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
InteractionHeightAttribute::InteractionHeightAttribute( const sword::ObjectAttributes& asnMsg )
    : height_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
InteractionHeightAttribute::~InteractionHeightAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void InteractionHeightAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_interaction_height()  )
        height_ = asnMsg.interaction_height().height();
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void InteractionHeightAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_interaction_height()->set_height( height_ );
}
