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
        initial_ = Color( static_cast< unsigned short >( color.red() ),
                          static_cast< unsigned short >( color.green() ),
                          static_cast< unsigned short >( color.blue() ) );
        alpha_ = color.alpha();
        current_ = initial_;
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

