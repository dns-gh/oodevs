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
#include "Population.h"

// -----------------------------------------------------------------------------
// Name: PopulationPositions constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
PopulationPositions::PopulationPositions( const Population& popu )
    : population_( popu )
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
    return population_.GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::IsAt
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool PopulationPositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    return population_.IsAt( pos, precision );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::IsIn
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool PopulationPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return population_.IsIn( rectangle );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::GetHeight
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
float PopulationPositions::GetHeight() const
{
    return population_.GetHeight();
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f PopulationPositions::GetBoundingBox() const
{
    return population_.GetBoundingBox();
}
