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

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
SupplyRouteAttribute::SupplyRouteAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
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
void SupplyRouteAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if( asnMsg.m.supply_routePresent )
    {
        bEquipped_  = asnMsg.supply_route.equipped;
        nMaxWeight_ = asnMsg.supply_route.max_weight;
        nWidth_     = asnMsg.supply_route.width;
        nLength_    = asnMsg.supply_route.length;
        nFlow_      = asnMsg.supply_route.flow_rate;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.m.supply_routePresent = 1;
    asnMsg.supply_route.equipped         = bEquipped_;
    asnMsg.supply_route.max_weight       = nMaxWeight_;
    asnMsg.supply_route.width            = nWidth_;
    asnMsg.supply_route.length           = nLength_;
    asnMsg.supply_route.flow_rate        = nFlow_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
//    delete asnMsg.u.logistic_route;
}

