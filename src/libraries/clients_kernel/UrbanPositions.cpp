// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************_

#include "clients_kernel_pch.h"
#include "UrbanPositions.h"
#include "Viewport_ABC.h"
#include "GlTools_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( std::vector< geometry::Point2f > positions,
                                const std::string& name, const UrbanColor_ABC& color )
    : name_             ( name )
    , color_            ( color )
    , selected_         ( false )
    , hasInfrastructure_( false )
{
    if( !positions.empty() )
    {
        if( positions.front() == positions.back() )
            positions.pop_back();
        polygon_ = geometry::Polygon2f( positions );
        boundingBox_ = polygon_.BoundingBox();
        barycenter_ = polygon_.Barycenter();
        area_ = polygon_.ComputeArea();
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions destructor
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
UrbanPositions::~UrbanPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Barycenter
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
const geometry::Point2f& UrbanPositions::Barycenter() const
{
    return barycenter_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::BoundingBox
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
const geometry::Rectangle2f& UrbanPositions::BoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ComputeArea
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
float UrbanPositions::ComputeArea() const
{
    return area_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Vertices
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
const std::vector< geometry::Point2f >& UrbanPositions::Vertices() const
{
    return polygon_.Vertices();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsInside
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
bool UrbanPositions::IsInside( const geometry::Point2f& point ) const
{
    return polygon_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::SetInfrastructurePresent
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
void UrbanPositions::SetInfrastructurePresent()
{
    hasInfrastructure_ = true;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ToggleSelection
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
void UrbanPositions::ToggleSelection()
{
    selected_ = !selected_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
        tools.DrawDecoratedPolygon( polygon_, color_, hasInfrastructure_ ? name_ : std::string(), 0, selected_ );
}
