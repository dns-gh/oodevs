// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ColorFactory.h"
#include "Color.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: ColorFactory constructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
ColorFactory::ColorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorFactory destructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
ColorFactory::~ColorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorFactory::CreateProperty
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
Paint_ABC* ColorFactory::CreateProperty( const std::string& color ) const
{
    if( color.empty() || color == "inherit" )
        return 0;
    return new Color( color );
}
