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
#include "clients_gui/UrbanDrawer.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include <urban/TerrainObject_ABC.h>

std::auto_ptr< urban::Drawer_ABC > UrbanPositions::drawer_;

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( const urban::TerrainObject_ABC& object/*, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location*/ )
    : object_( object )
{
    // NOTHING
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
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
geometry::Point2f UrbanPositions::GetPosition( bool ) const
{
    const geometry::Polygon2f* footprint = object_.GetFootprint();
    if( footprint )
        return footprint->Barycenter();
    return geometry::Point2f();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::GetHeight
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
float UrbanPositions::GetHeight( bool ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsAt
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
bool UrbanPositions::IsAt( const geometry::Point2f& pos, float /*precision*/, float /*adaptiveFactor*/ ) const
{
    return object_.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsIn
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
bool UrbanPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return ! GetBoundingBox().Intersect( rectangle ).IsEmpty();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::GetBoundingBox
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
geometry::Rectangle2f UrbanPositions::GetBoundingBox() const
{
    const geometry::Polygon2f* footprint = object_.GetFootprint();
    if( footprint )
        return footprint->BoundingBox();
    return geometry::Rectangle2f();
}
    
// -----------------------------------------------------------------------------
// Name: UrbanPositions::Accept
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
void UrbanPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    const geometry::Polygon2f* footprint = object_.GetFootprint();
    if( footprint )
        visitor.VisitLines( footprint->Vertices() );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    if( drawer_.get() == 0 )
        drawer_.reset( new gui::UrbanDrawer( tools ) );
    object_.Draw( *drawer_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool UrbanPositions::CanAggregate() const
{
    return false;
}
