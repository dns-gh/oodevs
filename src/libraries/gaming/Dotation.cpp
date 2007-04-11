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
    : type_( 0 )
    , quantity_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Dotation::Dotation( const DotationType& type, unsigned int quantity /* =0 */ )
    : type_( & type )
    , quantity_( quantity )
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
    dotation.quantity_ += rhs.quantity_;
    return dotation;
}

// -----------------------------------------------------------------------------
// Name: Dotation::operator-
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Dotation Dotation::operator-( const Dotation& rhs ) const
{
    Dotation dotation( *this );
    dotation.quantity_ -= rhs.quantity_;
    return dotation;
}
