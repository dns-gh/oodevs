// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanColor.h"
#include <urban/ColorAttribute.h>
#include <urban/TerrainObject_ABC.h>

// -----------------------------------------------------------------------------
// Name: UrbanColor constructor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
UrbanColor::UrbanColor( const urban::ColorAttribute& colorAttribute )
    : initial_( 200u, 200u, 200u )
    , current_( 200u, 200u, 200u )
    , alpha_  ( 0.7f )
{
    initial_ = Color( colorAttribute.Red(), colorAttribute.Green(), colorAttribute.Blue() );
    alpha_ = colorAttribute.Alpha();
    current_ = initial_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor destructor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
UrbanColor::~UrbanColor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::Red
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned short UrbanColor::Red() const
{
    return current_.red_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::Green
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned short UrbanColor::Green() const
{
    return current_.green_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::Blue
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned short UrbanColor::Blue() const
{
    return current_.blue_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::Alpha
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
float UrbanColor::Alpha() const
{
    return alpha_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::SetColor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
void UrbanColor::SetColor( unsigned short red, unsigned short green, unsigned short blue )
{
    current_ = Color( red, green, blue );
}

// -----------------------------------------------------------------------------
// Name: UrbanColor::Restore
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
void UrbanColor::Restore()
{
    current_ = initial_;
}
