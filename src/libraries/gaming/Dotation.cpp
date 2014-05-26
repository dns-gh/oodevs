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
    : type_                   ( 0 )
    , quantity_               ( 0 )
    , lowThresholdPercentage_ ( 0.f )
    , highThresholdPercentage_( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Dotation::Dotation( const DotationType& type, unsigned int quantity, float lowThresholdPercentage, float highThresholdPercentage )
    : type_                   ( &type )
    , quantity_               ( quantity )
    , lowThresholdPercentage_ ( lowThresholdPercentage )
    , highThresholdPercentage_( highThresholdPercentage )
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
    dotation.lowThresholdPercentage_ += rhs.lowThresholdPercentage_;
    dotation.highThresholdPercentage_ += rhs.highThresholdPercentage_;
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
    dotation.lowThresholdPercentage_ -= rhs.lowThresholdPercentage_;
    dotation.highThresholdPercentage_ -= rhs.highThresholdPercentage_;
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
    dotation.lowThresholdPercentage_ = -dotation.lowThresholdPercentage_;
    dotation.highThresholdPercentage_ = -dotation.highThresholdPercentage_;
    return dotation;
}
