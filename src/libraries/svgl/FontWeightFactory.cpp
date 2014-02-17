// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "FontWeightFactory.h"
#include "FontWeight.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: FontWeightFactory constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontWeightFactory::FontWeightFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontWeightFactory destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontWeightFactory::~FontWeightFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontWeightFactory::CreateProperty
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Property_ABC* FontWeightFactory::CreateProperty( const std::string& content ) const
{
    if( content.empty() || content == "inherit" )
        return 0;
    return new FontWeight( content );
}
