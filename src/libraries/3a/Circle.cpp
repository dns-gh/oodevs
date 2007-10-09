// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Circle.h"

// -----------------------------------------------------------------------------
// Name: Circle constructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
Circle::Circle( const std::vector< Position >& positions )
    : center_( positions.at( 0 ) )
    , radius_( center_.Distance( positions.at( 1 ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Circle destructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
Circle::~Circle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Circle::Contains
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
bool Circle::Contains( const Position& position ) const
{
    return center_.Distance( position ) <= radius_;
}
