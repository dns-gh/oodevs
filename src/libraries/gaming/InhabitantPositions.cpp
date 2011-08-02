// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "InhabitantPositions.h"
#include "Inhabitant.h"

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( const Inhabitant& inhabitant )
    : inhabitant_( inhabitant )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions destructor
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
InhabitantPositions::~InhabitantPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetPosition
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
geometry::Point2f InhabitantPositions::GetPosition( bool aggregated ) const
{
    return inhabitant_.GetPosition( aggregated );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::IsAt
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
bool InhabitantPositions::IsAt( const geometry::Point2f& /*pos*/, float /*precision*/ /* = 100.f*/, float /*adaptiveFactor*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::IsIn
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
bool InhabitantPositions::IsIn( const geometry::Rectangle2f& /*rectangle*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetHeight
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
float InhabitantPositions::GetHeight( bool /*aggregated*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetBoundingBox
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
geometry::Rectangle2f InhabitantPositions::GetBoundingBox() const
{
    return inhabitant_.GetBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Accept
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
void InhabitantPositions::Accept( kernel::LocationVisitor_ABC& /*visitor*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::CanAggregate
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
bool InhabitantPositions::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::IsAggregated
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
bool InhabitantPositions::IsAggregated() const
{
    return false;
}
