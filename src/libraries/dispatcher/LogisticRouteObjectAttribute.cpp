// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogisticRouteObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticRouteObjectAttribute::LogisticRouteObjectAttribute( const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( asnMsg )
    , bEquipped_         ( false )
    , nMaxWeight_        ( 0 )
    , nWidth_            ( 0 )
    , nLength_           ( 0 )
    , nFlow_             ( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticRouteObjectAttribute::~LogisticRouteObjectAttribute()
{
    // NOTHING
}  

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogisticRouteObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_logistic_route )
    {
        bEquipped_  = asnMsg.u.logistic_route->equipped;
        nMaxWeight_ = asnMsg.u.logistic_route->max_weight;
        nWidth_     = asnMsg.u.logistic_route->width;
        nLength_    = asnMsg.u.logistic_route->length;
        nFlow_      = asnMsg.u.logistic_route->flow_rate;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogisticRouteObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.logistic_route = new ASN1T_ObjectAttributesLogisticRoute();
    asnMsg.u.logistic_route->equipped         = bEquipped_;
    asnMsg.u.logistic_route->max_weight       = nMaxWeight_;
    asnMsg.u.logistic_route->width            = nWidth_;
    asnMsg.u.logistic_route->length           = nLength_;
    asnMsg.u.logistic_route->flow_rate        = nFlow_;
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogisticRouteObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    delete asnMsg.u.logistic_route;
}

