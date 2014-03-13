// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationPositions.h"
#include "Population.h"
#include "clients_kernel/LocationVisitor_ABC.h"

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
geometry::Point2f PopulationPositions::GetPosition( bool aggregated ) const
{
    return population_.GetPosition( aggregated );
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
float PopulationPositions::GetHeight( bool aggregated ) const
{
    return population_.GetHeight( aggregated);
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f PopulationPositions::GetBoundingBox() const
{
    return population_.GetBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void PopulationPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool PopulationPositions::CanAggregate() const
{
    return false;
}
