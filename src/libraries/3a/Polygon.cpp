// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Polygon.h"
#include "Position.h"

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
Polygon::Polygon( const std::vector< Position >& positions )
    : polygon_( Position::ToPolygon( positions ) )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Polygon destructor
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
Polygon::~Polygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon::Contains
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
bool Polygon::Contains( const Position& position ) const
{
    return position.IsInside( polygon_ );
}
