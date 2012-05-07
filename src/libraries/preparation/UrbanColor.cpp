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
