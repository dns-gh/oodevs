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
#include <boost/lexical_cast.hpp>

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
    : opacity_( std::min( 1.f, std::max( 0.f, boost::lexical_cast< float >( content ) ) ) )
{
    // NOTHING
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
