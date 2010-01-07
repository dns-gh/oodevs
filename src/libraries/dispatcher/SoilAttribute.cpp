// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "SoilAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SoilAttribute constructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
SoilAttribute::SoilAttribute( const Model& model, const ASN1T_UrbanAttributes& asnMsg )
: UrbanObjectAttribute_ABC( model, asnMsg )
, occupation_       ( 0. )
, trafficability_   ( 0. )
, isMultiple_       ( false )
, compoundClearing_ ("")
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: SoilAttribute destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
SoilAttribute::~SoilAttribute()
{
    //NOTHING
}  

// -----------------------------------------------------------------------------
// Name: SoilAttribute::Update
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void SoilAttribute::Update( const ASN1T_UrbanAttributes& asnMsg )
{
    if( asnMsg.m.soilPresent )
    {
        occupation_ = asnMsg.soil.occupation;
        trafficability_ = asnMsg.soil.trafficability;
        isMultiple_ = asnMsg.soil.multiple;
        compoundClearing_ = asnMsg.soil.compoundClearing;  
    }
}

// -----------------------------------------------------------------------------
// Name: SoilAttribute::Send
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void SoilAttribute::Send( ASN1T_UrbanAttributes& asnMsg ) const
{
    asnMsg.m.soilPresent = 1;

    asnMsg.soil.occupation = occupation_;
    asnMsg.soil.trafficability = trafficability_;
    asnMsg.soil.multiple = isMultiple_;
    asnMsg.soil.compoundClearing = compoundClearing_.c_str();
}

// -----------------------------------------------------------------------------
// Name: SoilAttribute::AsnDelete
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void SoilAttribute::AsnDelete( ASN1T_UrbanAttributes& /*asnMsg*/ ) const
{
    //NOTHING
}
