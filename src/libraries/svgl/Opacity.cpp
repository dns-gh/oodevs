// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Opacity.h"
#include <sstream>

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Opacity constructor
// Created: AGE 2007-04-26
// -----------------------------------------------------------------------------
Opacity::Opacity( float value /*= 1.f*/ )
    : opacity_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Opacity constructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
Opacity::Opacity( const std::string& content )
    : opacity_( 1 )
{
    std::stringstream str( content );
    str >> opacity_;
    if( opacity_ > 1 )
        opacity_ = 1;
    if( opacity_ < 0 )
        opacity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Opacity destructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
Opacity::~Opacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Opacity::Set
// Created: AGE 2007-05-25
// -----------------------------------------------------------------------------
void Opacity::Set( float value )
{
    opacity_ = value;
}

// -----------------------------------------------------------------------------
// Name: Opacity::GetValue
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
float Opacity::GetValue() const
{
    return opacity_;
}
    
// -----------------------------------------------------------------------------
// Name: Opacity::Clone
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
Opacity& Opacity::Clone() const
{
    return *new Opacity( *this );
}
