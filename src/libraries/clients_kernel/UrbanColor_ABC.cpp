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
    : initial_( 200u, 200u, 200u )
    , current_( 200u, 200u, 200u )
    , alpha_  ( 0.7f )
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
unsigned short UrbanColor_ABC::Red() const
{
    return current_.red_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::Green
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned short UrbanColor_ABC::Green() const
{
    return current_.green_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::Blue
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned short UrbanColor_ABC::Blue() const
{
    return current_.blue_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColor_ABC::Alpha
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
float UrbanColor_ABC::Alpha() const
{
    return alpha_;
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
void UrbanColor_ABC::SetColor( unsigned short red, unsigned short green, unsigned short blue )
{
    current_ = Color( red, green, blue );
}
