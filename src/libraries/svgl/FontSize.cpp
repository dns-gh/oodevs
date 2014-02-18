// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "FontSize.h"
#include <sstream>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: FontSize constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontSize::FontSize( const std::string& content )
    : size_( 1 )
{
    std::stringstream str( content );
    str >> size_;
}

// -----------------------------------------------------------------------------
// Name: FontSize constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontSize::FontSize( const FontSize& rhs )
    : size_( rhs.size_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontSize destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontSize::~FontSize()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontSize::ApplyScale
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
void FontSize::ApplyScale() const
{
    glScalef( size_, size_, size_ );
}

// -----------------------------------------------------------------------------
// Name: FontSize::Clone
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontSize& FontSize::Clone() const
{
    return *new FontSize( *this );
}
