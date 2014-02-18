// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "FontFamily.h"
#include "RenderingContext_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: FontFamily constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontFamily::FontFamily( const std::string& familyName )
    : familyName_( familyName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontFamily destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontFamily::~FontFamily()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontFamily::GetName
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
const std::string& FontFamily::GetName() const
{
    return familyName_;
}

// -----------------------------------------------------------------------------
// Name: FontFamily::Clone
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontFamily& FontFamily::Clone() const
{
    return *new FontFamily( familyName_ );
}
