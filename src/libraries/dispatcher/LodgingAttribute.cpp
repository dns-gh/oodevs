// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LodgingAttribute.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LodgingAttribute constructor
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::LodgingAttribute( const sword::ObjectAttributes& asnMsg )
    : capacity_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute destructor
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
LodgingAttribute::~LodgingAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Update
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_lodging() )
        capacity_ = asnMsg.lodging().capacity();
}

// -----------------------------------------------------------------------------
// Name: LodgingAttribute::Send
// Created: MMC 2011-05-04
// -----------------------------------------------------------------------------
void LodgingAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_lodging()->set_capacity( capacity_ );
}
