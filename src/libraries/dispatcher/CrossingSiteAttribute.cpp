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

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nWidth_            ( 0 )
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

}  

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if ( asnMsg.m.crossing_sitePresent )
    {
        nWidth_             = asnMsg.crossing_site.width;
        nDepth_             = asnMsg.crossing_site.depth;
        nSpeed_             = asnMsg.crossing_site.flow_rate;
        bNeedsConstruction_ = asnMsg.crossing_site.banks_require_fitting != 0;
    }
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.crossing_site.width                 = nWidth_;
    asnMsg.crossing_site.depth                 = nDepth_;
    asnMsg.crossing_site.flow_rate             = nSpeed_;
    asnMsg.crossing_site.banks_require_fitting = bNeedsConstruction_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
//    delete asnMsg.u.crossing_site;
}
