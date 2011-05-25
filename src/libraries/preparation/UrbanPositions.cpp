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
#include <urban/GeometryAttribute.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/PhysicalAttribute.h>

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( const urban::TerrainObject_ABC& object, const kernel::UrbanColor_ABC& color )
    : object_  ( object )
    , color_   ( color )
    , selected_( false )
    , height_  ( 0u )
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
    return object_.Get< urban::GeometryAttribute >().Geometry().Barycenter();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::BoundingBox
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
geometry::Rectangle2f UrbanPositions::BoundingBox() const
{
    return object_.Get< urban::GeometryAttribute >().Geometry().BoundingBox();
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
    return object_.Get< urban::GeometryAttribute >().Geometry().Vertices();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    tools.DrawDecoratedPolygon( object_.Get< urban::GeometryAttribute >().Geometry(), color_, object_.GetName(), height_, selected_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ToggleSelection
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
void UrbanPositions::ToggleSelection()
{
    selected_ = !selected_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ComputeArea
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
float UrbanPositions::ComputeArea() const
{
    return object_.Get< urban::GeometryAttribute >().Geometry().ComputeArea();

}
