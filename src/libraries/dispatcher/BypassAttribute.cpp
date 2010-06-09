// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "protocol/protocol.h"
#include "BypassAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( const Common::MsgObjectAttributes& asnMsg )
    : nPercentageBypassing_( asnMsg.bypass().percentage() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
BypassAttribute::~BypassAttribute()
{
    // NOTHING
}  

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void BypassAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if( asnMsg.has_bypass()  )
        nPercentageBypassing_ = asnMsg.bypass().percentage();
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void BypassAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_bypass()->set_percentage( nPercentageBypassing_ );
}
