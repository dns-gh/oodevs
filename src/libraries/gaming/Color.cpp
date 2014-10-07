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
#include <boost/tuple/tuple_comparison.hpp>

// -----------------------------------------------------------------------------
// Name: Color constructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
Color::Color( const sword::RgbColor& color )
    : base_( boost::tuples::make_tuple( color.red(), color.green(), color.blue() ) )
{
    // NOTHING
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
// Name: Color::GetColor
// Created: SLI 2014-10-01
// -----------------------------------------------------------------------------
const kernel::Color_ABC::T_Color& Color::GetColor() const
{
    if( !color_ )
        return base_;
    return *color_;
}

// -----------------------------------------------------------------------------
// Name: Color::GetBaseColor
// Created: SLI 2014-10-02
// -----------------------------------------------------------------------------
const kernel::Color_ABC::T_Color& Color::GetBaseColor() const
{
    return base_;
}

// -----------------------------------------------------------------------------
// Name: Color::ChangeColor
// Created: SLI 2014-10-02
// -----------------------------------------------------------------------------
void Color::ChangeColor( const T_Color& color )
{
    if( base_ == color )
        Clear();
    else
        color_ = color;
}
