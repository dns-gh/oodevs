// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Color.h"

using namespace gui;

Color::Color()
{
    // NOTHING
}

Color::~Color()
{
    // NOTHING
}

bool Color::IsOverriden() const
{
    return color_;
}

const kernel::Color_ABC::T_Color& Color::GetColor() const
{
    if( !color_ )
        throw MASA_EXCEPTION( "Accessing invalid color" );
    return *color_;
}

void Color::ChangeColor( const T_Color& color )
{
    color_ = color;
}

void Color::Clear()
{
    color_ = boost::none;
}
