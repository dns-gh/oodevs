// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "FontSizeFactory.h"
#include "FontSize.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: FontSizeFactory constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontSizeFactory::FontSizeFactory()
{
    // NOTHING
}
 
// -----------------------------------------------------------------------------
// Name: FontSizeFactory destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontSizeFactory::~FontSizeFactory()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: FontSizeFactory::CreateProperty
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Property_ABC* FontSizeFactory::CreateProperty( const std::string& content ) const
{
    if( content.empty() || content == "inherit" )
        return 0;
    return new FontSize( content );
}
