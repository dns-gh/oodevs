// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Dotation.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Dotation::Dotation()
    : type_               ( 0 )
    , quantity_           ( 0 )
    , thresholdPercentage_( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Dotation::Dotation( const DotationType& type, unsigned int quantity /* =0 */, float thresholdPercentage /*= 0.f*/ )
    : type_               ( &type )
    , quantity_           ( quantity )
    , thresholdPercentage_( thresholdPercentage )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Dotation::~Dotation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation::operator+
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Dotation Dotation::operator+( const Dotation& rhs ) const
{
    Dotation dotation( *this );
    if( dotation.quantity_ < std::numeric_limits< int >::max() )
        dotation.quantity_ += rhs.quantity_;
    dotation.thresholdPercentage_ += rhs.thresholdPercentage_;
    return dotation;
}

// -----------------------------------------------------------------------------
// Name: Dotation::operator-
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Dotation Dotation::operator-( const Dotation& rhs ) const
{
    Dotation dotation( *this );
    if( dotation.quantity_ < std::numeric_limits< int >::max() )
        dotation.quantity_ -= rhs.quantity_;
    dotation.thresholdPercentage_ -= rhs.thresholdPercentage_;
    return dotation;
}

// -----------------------------------------------------------------------------
// Name: Dotation::operator-
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
Dotation Dotation::operator-() const
{
    Dotation dotation( *this );
    dotation.quantity_ = -dotation.quantity_;
    dotation.thresholdPercentage_ = -dotation.thresholdPercentage_;
    return dotation;
}
