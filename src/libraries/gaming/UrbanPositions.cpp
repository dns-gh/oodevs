// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanPositions.h"
#include "clients_kernel/GlTools_ABC.h"
#include "urban/TerrainObject_ABC.h"

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( const urban::TerrainObject_ABC& object, const sword::Location& message, const kernel::CoordinateConverter_ABC& converter )
    : LocationPositions( converter )
    , object_( object )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions destructor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::~UrbanPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::GetPosition
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
geometry::Point2f UrbanPositions::GetPosition( bool /*aggregated*/ ) const
{
    return object_.GetFootprint()->Barycenter();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    tools.DrawDecoratedPolygon( *object_.GetFootprint(), object_.GetDecoration() );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsAt
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
bool UrbanPositions::IsAt( const geometry::Point2f& pos, float /*precision*/, float /*adaptiveFactor*/ ) const
{
    return object_.IsInside( pos );
}
