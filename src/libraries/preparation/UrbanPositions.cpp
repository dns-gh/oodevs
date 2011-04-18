// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanPositions.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include <urban/TerrainObject_ABC.h>
#include <urban/PhysicalAttribute.h>

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( const urban::TerrainObject_ABC& object )
    : object_( object )
    , height_( 0u )
{
    const urban::PhysicalAttribute* pPhysical = object.Retrieve< urban::PhysicalAttribute >();;
    if( pPhysical && pPhysical->GetArchitecture() )
        height_ = static_cast< unsigned int >( pPhysical->GetArchitecture()->GetHeight() );
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
// Name: UrbanPositions::Barycenter
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
geometry::Point2f UrbanPositions::Barycenter() const
{
    return object_.GetFootprint()->Barycenter();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::BoundingBox
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
geometry::Rectangle2f UrbanPositions::BoundingBox() const
{
    return object_.GetFootprint()->BoundingBox();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsInside
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
bool UrbanPositions::IsInside( const geometry::Point2f& point ) const
{
    return object_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Vertices
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
const std::vector< geometry::Point2f >& UrbanPositions::Vertices() const
{
    return object_.GetFootprint()->Vertices();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    tools.DrawDecoratedPolygon( *object_.GetFootprint(), object_.GetDecoration(), object_.GetName(), height_ );
}
