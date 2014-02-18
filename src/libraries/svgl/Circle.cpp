// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Circle.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Circle constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Circle::Circle( xml::xistream& input, References_ABC& references, const PropertyFactory& factory  )
    : Ellipse( input, references, factory, ReadCenter( input ), ReadRadius( input ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Circle destructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Circle::~Circle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Circle::ReadCenter
// Created: AGE 2006-08-30
// -----------------------------------------------------------------------------
geometry::Point Circle::ReadCenter( xml::xistream& input )
{
    float x = 0, y = 0;
    input >> xml::optional() >> xml::attribute( "cx", x )
          >> xml::optional() >> xml::attribute( "cy", y );
    return geometry::Point( x, y );
}

// -----------------------------------------------------------------------------
// Name: Circle::ReadRadius
// Created: AGE 2006-08-30
// -----------------------------------------------------------------------------
float Circle::ReadRadius( xml::xistream& input )
{
    float radius;
    input >> xml::attribute( "r", radius );
    return radius;
}
