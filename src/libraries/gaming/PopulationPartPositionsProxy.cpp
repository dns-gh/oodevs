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
geometry::Point2f PopulationPartPositionsProxy::GetPosition() const
{
    return positions_.GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy::GetHeight
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
float PopulationPartPositionsProxy::GetHeight() const
{
    return positions_.GetHeight();
}

// -----------------------------------------------------------------------------
// Name: PopulationPartPositionsProxy::IsAt
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
bool PopulationPartPositionsProxy::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    return positions_.IsAt( pos, precision );
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
