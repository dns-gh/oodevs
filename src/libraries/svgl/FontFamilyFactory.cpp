// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "FontFamilyFactory.h"
#include "FontFamily.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: FontFamilyFactory constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontFamilyFactory::FontFamilyFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontFamilyFactory destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontFamilyFactory::~FontFamilyFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontFamilyFactory::CreateProperty
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Property_ABC* FontFamilyFactory::CreateProperty( const std::string& content ) const
{
    if( content.empty() || content == "inherit" )
        return 0;
    return new FontFamily( content );
}
