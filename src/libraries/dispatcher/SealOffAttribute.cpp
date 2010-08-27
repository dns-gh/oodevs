// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "protocol/protocol.h"
#include "SealOffAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SealOffAttribute constructor
// Created: MGD 2010-08-12
// -----------------------------------------------------------------------------
SealOffAttribute::SealOffAttribute( const Common::MsgObjectAttributes& asnMsg )
    : nSealOffLevel_( asnMsg.bypass().percentage() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute destructor
// Created: MGD 2010-08-12
// -----------------------------------------------------------------------------
SealOffAttribute::~SealOffAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::Update
// Created: MGD 2010-08-12
// -----------------------------------------------------------------------------
void SealOffAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if( asnMsg.has_bypass()  )
        nSealOffLevel_ = asnMsg.bypass().percentage();
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::Send
// Created: MGD 2010-08-12
// -----------------------------------------------------------------------------
void SealOffAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_bypass()->set_percentage( nSealOffLevel_ );
}
