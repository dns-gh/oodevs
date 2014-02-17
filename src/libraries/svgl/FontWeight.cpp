// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "FontWeight.h"
#include <sstream>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: FontWeight constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontWeight::FontWeight( const std::string& content )
    : value_( 500 )
{
    if( content.find( "00" ) != std::string::npos )
    {
        std::stringstream str( content );
        str >> value_;
    }
    else if( content == "thin" )
        value_ = 100;
    else if( content == "light" )
        value_ = 300;
    else if( content == "normal" )
        value_ = 400;
    else if( content == "medium" )
        value_ = 500;
    else if( content == "bold" )
        value_ = 700;
    else if( content == "heavy" )
        value_ = 900;
}

// -----------------------------------------------------------------------------
// Name: FontWeight constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontWeight::FontWeight( const FontWeight& rhs )
    : value_( rhs.value_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontWeight destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontWeight::~FontWeight()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FontWeight::Clone
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
FontWeight& FontWeight::Clone() const
{
    return *new FontWeight( *this );
}

// -----------------------------------------------------------------------------
// Name: FontWeight::GetValue
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
int FontWeight::GetValue() const
{
    return value_;
}
