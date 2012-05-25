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
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: UrbanColor constructor
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
UrbanColor::UrbanColor( xml::xistream& xis )
    : initial_( 200u, 200u, 200u )
    , current_( 200u, 200u, 200u )
    , alpha_  ( 0.7f )
{
    if( xis.has_child( "color" ) )
    {
        xis >> xml::start( "color" );
        initial_ = Color( xis.attribute< unsigned short >( "red" ), xis.attribute< unsigned short >( "green" ),
                          xis.attribute< unsigned short >( "blue" ) );
        alpha_ = xis.attribute< float >( "alpha" );
        current_ = initial_;
        xis >> xml::end;
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

// -----------------------------------------------------------------------------
// Name: UrbanColor::SerializeAttributes
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanColor::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "color" )
            << xml::attribute< unsigned short >( "red", initial_.red_ )
            << xml::attribute< unsigned short >( "green", initial_.green_ )
            << xml::attribute< unsigned short >( "blue", initial_.blue_ )
            << xml::attribute< float >( "alpha", Alpha() )
        << xml::end;
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
