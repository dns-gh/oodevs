// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UrbanColor_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC constructor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
UrbanColor_ABC::UrbanColor_ABC()
    : initial_( 80, 80, 80, 180 )
    , current_( 80, 80, 80, 180 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC destructor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
UrbanColor_ABC::~UrbanColor_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::Red
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
int UrbanColor_ABC::Red() const
{
    return current_.red_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::Green
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
int UrbanColor_ABC::Green() const
{
    return current_.green_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::Blue
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
int UrbanColor_ABC::Blue() const
{
    return current_.blue_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::Alpha
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
float UrbanColor_ABC::Alpha() const
{
    return current_.alpha_ / 255.f;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::Restore
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
void UrbanColor_ABC::Restore()
{
    current_ = initial_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::SetColor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
void UrbanColor_ABC::SetColor( int red, int green, int blue, int alpha )
{
    current_ = UrbanBlockColor( red, green, blue, alpha );
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::SetColor
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void UrbanColor_ABC::SetColor( const UrbanBlockColor& color )
{
    current_ = color;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::GetColor
// Created: ABR 2013-02-19
// -----------------------------------------------------------------------------
UrbanBlockColor UrbanColor_ABC::GetColor() const
{
    return current_;
}
