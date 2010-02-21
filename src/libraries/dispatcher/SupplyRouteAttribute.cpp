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
SupplyRouteAttribute::SupplyRouteAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , bEquipped_         ( false )
    , nMaxWeight_        ( 0 )
    , nWidth_            ( 0 )
    , nLength_           ( 0 )
    , nFlow_             ( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
SupplyRouteAttribute::~SupplyRouteAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if( asnMsg.has_supply_route() )
    {
        bEquipped_  = asnMsg.supply_route().equipped();
        nMaxWeight_ = asnMsg.supply_route().max_weight();
        nWidth_     = asnMsg.supply_route().width();
        nLength_    = asnMsg.supply_route().length();
        nFlow_      = asnMsg.supply_route().flow_rate();
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_supply_route()->set_equipped         ( bEquipped_ );
    asnMsg.mutable_supply_route()->set_max_weight       ( nMaxWeight_ );
    asnMsg.mutable_supply_route()->set_width            ( nWidth_ );
    asnMsg.mutable_supply_route()->set_length           ( nLength_ );
    asnMsg.mutable_supply_route()->set_flow_rate        ( nFlow_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Delete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Delete( Common::MsgObjectAttributes& /*asnMsg*/ ) const
{
//    delete asnMsg().logistic_route;
}

