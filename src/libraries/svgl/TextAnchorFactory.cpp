// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "TextAnchorFactory.h"
#include "TextAnchor.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: TextAnchorFactory constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TextAnchorFactory::TextAnchorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TextAnchorFactory destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TextAnchorFactory::~TextAnchorFactory()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: TextAnchorFactory::CreateProperty
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
Property_ABC* TextAnchorFactory::CreateProperty( const std::string& content ) const
{
    if( content.empty() || content == "inherit" )
        return 0;
    return new TextAnchor( content );
}
