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
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ColorAttribute constructor
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
ColorAttribute::ColorAttribute( const Model& model, const MsgsSimToClient::MsgUrbanAttributes& message )
    : UrbanObjectAttribute_ABC( model, message )
    , color_( new ColorRGBA( static_cast< unsigned short >( message.color().red() ),
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
    delete color_;
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute::Update
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void ColorAttribute::Update( const MsgsSimToClient::MsgUrbanAttributes& /*message*/ )
{
   // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute::Send
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void ColorAttribute::Send( MsgsSimToClient::MsgUrbanAttributes& message ) const
{
    MsgsSimToClient::MsgColorRGBA* color = message.mutable_color();
    color->set_red( color_->Red() );
    color->set_green( color_->Green() );
    color->set_blue( color_->Blue() );
    color->set_alpha( color_->Alpha() );
}

// -----------------------------------------------------------------------------
// Name: ColorAttribute::AsnDelete
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void ColorAttribute::AsnDelete( MsgsSimToClient::MsgUrbanAttributes& /*message*/ ) const
{
    // NOTHING
}
