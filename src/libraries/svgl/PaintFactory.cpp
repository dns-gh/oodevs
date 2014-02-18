// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "PaintFactory.h"
#include "ColorNone.h"
#include "ColorCurrent.h"
#include "PaintReference.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: PaintFactory constructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
PaintFactory::PaintFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PaintFactory destructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
PaintFactory::~PaintFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PaintFactory::CreateProperty
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
Paint_ABC* PaintFactory::CreateProperty( const std::string& color ) const
{
    if( color.empty() || color == "inherit" )
        return 0;
    if( color == "none" )
        return new ColorNone();
    if( color == "currentColor" )
        return new ColorCurrent();
    if( color.size() > 4 && color.substr( 0, 4 ) == "url(" )
        return new PaintReference( color );
    return ColorFactory::CreateProperty( color );
}
