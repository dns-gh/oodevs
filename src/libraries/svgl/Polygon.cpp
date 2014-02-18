// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Polygon.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Polygon::Polygon( xml::xistream& input, References_ABC& references, const PropertyFactory& factory )
    : Path( input, references, factory, CreatePath( input ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon destructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Polygon::~Polygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon::CreatePath
// Created: AGE 2006-08-30
// -----------------------------------------------------------------------------
std::string Polygon::CreatePath( xml::xistream& input )
{
    std::string points;
    input >> xml::attribute( "points", points );
    return "M " + points + " z";
}
