// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "CrossingSiteObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CrossingSiteObjectAttribute::CrossingSiteObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nWidth_            ( 0 )
    , nDepth_            ( 0 )
    , nSpeed_            ( 0 )
    , bNeedsConstruction_( false ) 
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CrossingSiteObjectAttribute::~CrossingSiteObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void CrossingSiteObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_crossing_site )
    {
        nWidth_             = asnMsg.u.crossing_site->width;
        nDepth_             = asnMsg.u.crossing_site->depth;
        nSpeed_             = asnMsg.u.crossing_site->flow_rate;
        bNeedsConstruction_ = asnMsg.u.crossing_site->banks_require_fitting;
    }
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void CrossingSiteObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.crossing_site = new ASN1T_ObjectAttributesCrossingSite();
    asnMsg.u.crossing_site->width                 = nWidth_;
    asnMsg.u.crossing_site->depth                 = nDepth_;
    asnMsg.u.crossing_site->flow_rate             = nSpeed_;
    asnMsg.u.crossing_site->banks_require_fitting = bNeedsConstruction_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void CrossingSiteObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    delete asnMsg.u.crossing_site;
}
