// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SupplyRouteAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
SupplyRouteAttribute::SupplyRouteAttribute( const Common::MsgObjectAttributes& message )
    : bEquipped_  ( false )
    , nMaxWeight_ ( 0 )
    , nWidth_     ( 0 )
    , nLength_    ( 0 )
    , nFlow_      ( 0 )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
SupplyRouteAttribute::~SupplyRouteAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Update( const Common::MsgObjectAttributes& message )
{
    if( message.has_supply_route() )
    {
        bEquipped_  = message.supply_route().equipped();
        nMaxWeight_ = message.supply_route().max_weight();
        nWidth_     = message.supply_route().width();
        nLength_    = message.supply_route().length();
        nFlow_      = message.supply_route().flow_rate();
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Send( Common::MsgObjectAttributes& message ) const
{
    message.mutable_supply_route()->set_equipped  ( bEquipped_ );
    message.mutable_supply_route()->set_max_weight( nMaxWeight_ );
    message.mutable_supply_route()->set_width     ( nWidth_ );
    message.mutable_supply_route()->set_length    ( nLength_ );
    message.mutable_supply_route()->set_flow_rate ( nFlow_ );
}
