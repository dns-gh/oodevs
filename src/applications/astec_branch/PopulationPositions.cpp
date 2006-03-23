// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationPositions.h"

// -----------------------------------------------------------------------------
// Name: PopulationPositions constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
PopulationPositions::PopulationPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions destructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
PopulationPositions::~PopulationPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::GetPosition
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
geometry::Point2f PopulationPositions::GetPosition() const
{
    return geometry::Point2f();
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::IsAt
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool PopulationPositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::IsIn
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool PopulationPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return true;
}
