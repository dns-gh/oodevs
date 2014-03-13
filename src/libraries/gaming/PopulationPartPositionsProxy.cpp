// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationPartPositionsProxy.h"

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy constructor
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
PopulationPartPositionsProxy::PopulationPartPositionsProxy( const kernel::Positions& positions )
    : positions_( positions )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy destructor
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
PopulationPartPositionsProxy::~PopulationPartPositionsProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy::GetPosition
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
geometry::Point2f PopulationPartPositionsProxy::GetPosition( bool aggregated ) const
{
    return positions_.GetPosition( aggregated );
}

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy::GetHeight
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
float PopulationPartPositionsProxy::GetHeight( bool aggregated ) const
{
    return positions_.GetHeight( aggregated );
}

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy::IsIn
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
bool PopulationPartPositionsProxy::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return positions_.IsIn( rectangle );
}

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy::GetBoundingBox
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
geometry::Rectangle2f PopulationPartPositionsProxy::GetBoundingBox() const
{
    return positions_.GetBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void PopulationPartPositionsProxy::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    positions_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool PopulationPartPositionsProxy::CanAggregate() const
{
    return false;
}
