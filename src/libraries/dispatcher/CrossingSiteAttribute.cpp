// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "CrossingSiteAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute( const Common::ObjectAttributes& asnMsg )
    : nWidth_            ( 0 )
    , nDepth_            ( 0 )
    , nSpeed_            ( 0 )
    , bNeedsConstruction_( false )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CrossingSiteAttribute::~CrossingSiteAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::Update( const Common::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_crossing_site() )
    {
        nWidth_             = asnMsg.crossing_site().width();
        nDepth_             = asnMsg.crossing_site().depth();
        nSpeed_             = asnMsg.crossing_site().flow_rate();
        bNeedsConstruction_ = asnMsg.crossing_site().banks_require_fitting() != 0;
    }
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::Send( Common::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_crossing_site()->set_width                ( nWidth_ );
    asnMsg.mutable_crossing_site()->set_depth                ( nDepth_ );
    asnMsg.mutable_crossing_site()->set_flow_rate            ( nSpeed_ );
    asnMsg.mutable_crossing_site()->set_banks_require_fitting( bNeedsConstruction_ );
}
