// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanColorAttribute.h"

// -----------------------------------------------------------------------------
// Name: UrbanColorAttribute constructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
UrbanColorAttribute::UrbanColorAttribute( xml::xistream& xis )
    : red_  ( 200 )
    , green_( 200 )
    , blue_ ( 200 )
    , alpha_( 0.7f )
{
    xis >> xml::optional >> xml::start( "color" )
            >> xml::optional >> xml::attribute( "red", red_ )
            >> xml::optional >> xml::attribute( "green", green_ )
            >> xml::optional >> xml::attribute( "blue", blue_ )
            >> xml::optional >> xml::attribute( "alpha", alpha_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: UrbanColorAttribute destructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
UrbanColorAttribute::~UrbanColorAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanColorAttribute::Red
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
unsigned short UrbanColorAttribute::Red() const
{
    return red_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColorAttribute::Green
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
unsigned short UrbanColorAttribute::Green() const
{
    return green_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColorAttribute::Blue
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
unsigned short UrbanColorAttribute::Blue() const
{
    return blue_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColorAttribute::Alpha
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
float UrbanColorAttribute::Alpha() const
{
    return alpha_;
}
