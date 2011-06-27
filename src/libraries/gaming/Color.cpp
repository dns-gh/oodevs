// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Color.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Color constructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
Color::Color( const sword::RgbColor& color )
{
    color_ = boost::tuples::make_tuple( color.red(), color.green(), color.blue() );
}

// -----------------------------------------------------------------------------
// Name: Color destructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
Color::~Color()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Color::IsOverride
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
bool Color::IsOverride() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Color::GetColor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
const Color::T_Color& Color::GetColor() const
{
    return color_;
}

// -----------------------------------------------------------------------------
// Name: Color::ChangeColor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void Color::ChangeColor( const T_Color& /*color*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Color::Clear
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void Color::Clear()
{
    // NOTHING
}
