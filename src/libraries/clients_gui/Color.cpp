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

// -----------------------------------------------------------------------------
// Name: Color constructor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
Color::Color()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Color destructor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
Color::~Color()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Color::IsOverriden
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
bool Color::IsOverriden() const
{
    return color_;
}

// -----------------------------------------------------------------------------
// Name: Color::GetColor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
const kernel::Color_ABC::T_Color& Color::GetColor() const
{
    if( !color_ )
        throw MASA_EXCEPTION( "Accessing invalid color" );
    return *color_;
}

// -----------------------------------------------------------------------------
// Name: Color::ChangeColor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
void Color::ChangeColor( const T_Color& color )
{
    color_ = color;
}

// -----------------------------------------------------------------------------
// Name: Color::Clear
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
void Color::Clear()
{
    color_ = boost::none;
}
