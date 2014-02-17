// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "PolyLine.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: PolyLine constructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
PolyLine::PolyLine( xml::xistream& input, References_ABC& references, const PropertyFactory& factory )
    : Path( input, references, factory, CreatePath( input ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PolyLine destructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
PolyLine::~PolyLine()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: PolyLine::CreatePath
// Created: AGE 2006-08-30
// -----------------------------------------------------------------------------
std::string PolyLine::CreatePath( xml::xistream& input )
{
    std::string points;
    input >> xml::attribute( "points", points );
    return "M " + points;
}
