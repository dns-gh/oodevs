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
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Architecture_ABC.h"

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( xml::xistream& xis, const std::string& name, const kernel::UrbanColor_ABC& color,
                                const kernel::CoordinateConverter_ABC& converter, const kernel::Architecture_ABC& architecture )
    : name_             ( name )
    , color_            ( color )
    , selected_         ( false )
    , height_           ( architecture.GetHeight() )
    , hasInfrastructure_( false )
{
    if( xis.has_child( "footprint" ) )
    {
        std::vector< geometry::Point2f > positions;
        xis >> xml::start( "footprint" )
                >> xml::list( "point", *this, &UrbanPositions::ReadPoint, positions, converter )
            >> xml::end;
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
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
UrbanPositions::~UrbanPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ReadPoint
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
void UrbanPositions::ReadPoint( xml::xistream& xis, std::vector< geometry::Point2f >& positions, const kernel::CoordinateConverter_ABC& converter ) const
{
    positions.push_back( converter.ConvertToXY( xis.attribute< std::string >( "location" ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Barycenter
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
const geometry::Point2f& UrbanPositions::Barycenter() const
{
    return barycenter_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::BoundingBox
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
const geometry::Rectangle2f& UrbanPositions::BoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsInside
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
bool UrbanPositions::IsInside( const geometry::Point2f& point ) const
{
    return polygon_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Vertices
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
const std::vector< geometry::Point2f >& UrbanPositions::Vertices() const
{
    return polygon_.Vertices();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
        tools.DrawDecoratedPolygon( polygon_, color_, hasInfrastructure_ ? name_ : std::string(), 0, selected_ );
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
// Name: UrbanPositions::SetInfrastructurePresent
// Created: JSR 2011-09-19
// -----------------------------------------------------------------------------
void UrbanPositions::SetInfrastructurePresent()
{
    hasInfrastructure_ = true;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ComputeArea
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
float UrbanPositions::ComputeArea() const
{
    return area_;
}
