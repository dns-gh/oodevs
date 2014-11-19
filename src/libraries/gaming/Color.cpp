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

Color::Color( const sword::RgbColor& color )
    : base_( boost::tuples::make_tuple( color.red(), color.green(), color.blue() ) )
{
    // NOTHING
}

Color::~Color()
{
    // NOTHING
}

const kernel::Color_ABC::T_Color& Color::GetColor() const
{
    if( !color_ )
        return base_;
    return *color_;
}

const kernel::Color_ABC::T_Color& Color::GetBaseColor() const
{
    return base_;
}

void Color::ChangeColor( const T_Color& color )
{
    if( base_ == color )
        Clear();
    else
        color_ = color;
}
