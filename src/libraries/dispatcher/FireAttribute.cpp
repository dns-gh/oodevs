// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "FireAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( const Common::MsgObjectAttributes& asnMsg )
    : heat_( asnMsg.fire().heat() )
    , nFireClass_( asnMsg.fire().class_id() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute destructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
FireAttribute::~FireAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Update
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
void FireAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if ( asnMsg.has_fire() )
    {
        heat_       = asnMsg.fire().heat();
        nFireClass_ = asnMsg.fire().class_id();
    }  
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Send
// Created: RFT 2006-09-27
// -----------------------------------------------------------------------------
void FireAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_fire()->set_heat( heat_ );
    asnMsg.mutable_fire()->set_class_id( nFireClass_ );
}
