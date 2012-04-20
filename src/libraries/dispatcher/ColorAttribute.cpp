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
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ColorAttribute constructor
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
ColorAttribute::ColorAttribute( const sword::UrbanAttributes& message )
    : red_  ( message.color().red() )
    , green_( message.color().green() )
    , blue_ ( message.color().blue() )
    , alpha_( message.color().alpha() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute destructor
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
ColorAttribute::~ColorAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute::Update
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void ColorAttribute::Update( const sword::UrbanAttributes& message )
{
   if( message.has_color() )
   {
        red_ =  message.color().red();
        green_ = message.color().green();
        blue_ = message.color().blue();
        alpha_ = message.color().alpha();
   }
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute::Send
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void ColorAttribute::Send( sword::UrbanAttributes& message ) const
{
    sword::RgbaColor& color = *message.mutable_color();
    color.set_red( red_ );
    color.set_green( green_ );
    color.set_blue( blue_ );
    color.set_alpha( alpha_ );
}
