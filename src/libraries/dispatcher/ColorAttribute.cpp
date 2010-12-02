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
#include <urban/ColorRGBA.h>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ColorAttribute constructor
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
ColorAttribute::ColorAttribute( const MsgsSimToClient::UrbanAttributes& message )
    : color_( new ColorRGBA( static_cast< unsigned short >( message.color().red() ),
                             static_cast< unsigned short >( message.color().green() ),
                             static_cast< unsigned short >( message.color().blue() ),
                             message.color().alpha() ) )
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
void ColorAttribute::Update( const MsgsSimToClient::UrbanAttributes& message )
{
   if( message.has_color() )
   {
       color_.reset( new ColorRGBA( static_cast< unsigned short >( message.color().red() ),
                                    static_cast< unsigned short >( message.color().green() ),
                                    static_cast< unsigned short >( message.color().blue() ),
                                    message.color().alpha() ) );
   }
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute::Send
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void ColorAttribute::Send( MsgsSimToClient::UrbanAttributes& message ) const
{
    Common::RgbaColor& color = *message.mutable_color();
    color.set_red( color_->Red() );
    color.set_green( color_->Green() );
    color.set_blue( color_->Blue() );
    color.set_alpha( color_->Alpha() );
}
