// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ColorAttribute.h"
#include "urban/ColorRGBA.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ColorAttribute constructor
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
ColorAttribute::ColorAttribute( const Model& model, const ASN1T_UrbanAttributes& asnMsg )
    : UrbanObjectAttribute_ABC( model, asnMsg )
    , color_       ( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute destructor
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
ColorAttribute::~ColorAttribute()
{
    delete color_;
}  

// -----------------------------------------------------------------------------
// Name: ColorAttribute::Update
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void ColorAttribute::Update( const ASN1T_UrbanAttributes& asnMsg )
{
    if ( color_ == 0 )
    {
        color_ = new ColorRGBA( asnMsg.color.red, asnMsg.color.green, asnMsg.color.blue, asnMsg.color.alpha );
    }
    else
    {
        color_->Red( asnMsg.color.red );
        color_->Green( asnMsg.color.green );
        color_->Blue( asnMsg.color.blue );
        color_->Alpha( asnMsg.color.alpha );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute::Send
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void ColorAttribute::Send( ASN1T_UrbanAttributes& asnMsg ) const
{
    asnMsg.m.colorPresent = 1;
    asnMsg.color.red = color_->Red();
    asnMsg.color.green = color_->Green();
    asnMsg.color.blue = color_->Blue();
    asnMsg.color.alpha = color_->Alpha();
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute::AsnDelete
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void ColorAttribute::AsnDelete( ASN1T_UrbanAttributes& /*asnMsg*/ ) const
{
    //NOTHING
}
