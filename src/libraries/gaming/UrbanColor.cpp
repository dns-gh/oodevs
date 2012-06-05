// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanColor.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: UrbanColor constructor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
UrbanColor::UrbanColor( const sword::UrbanAttributes& message )
{
    if( message.has_color() )
    {
        const sword::RgbaColor& color = message.color();
        SetColor( color.red(), color.green(), color.blue(), static_cast< int >( color.alpha() * 255 ) );
        initial_ = current_;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanColor destructor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
UrbanColor::~UrbanColor()
{
    // NOTHING
}
