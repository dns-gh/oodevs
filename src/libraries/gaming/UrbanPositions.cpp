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
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( const sword::Location& location, const sword::UrbanAttributes& attributes,
                                const kernel::CoordinateConverter_ABC& converter, const std::string& name, const kernel::UrbanColor_ABC& color )
    : name_             ( name )
    , color_            ( color )
    , selected_         ( false )
    , height_           ( 0u )
    , hasInfrastructure_( false )
{
    std::vector< geometry::Point2f > positions;
    for( int i = 0; i < location.coordinates().elem_size(); ++i )
        positions.push_back( converter.ConvertToXY( location.coordinates().elem( i ) ) );
    if( positions.empty() )
        throw std::runtime_error( "UrbanPositions cannot be created with empty location" );
    if( positions.front() == positions.back() )
        positions.pop_back();
    polygon_ = geometry::Polygon2f( positions );
    boundingBox_ = polygon_.BoundingBox();
    barycenter_ = polygon_.Barycenter();
    area_ = polygon_.ComputeArea();
    if( attributes.has_architecture() )
        height_ = static_cast< unsigned int >( attributes.architecture().height() );
    if( !IsInside( barycenter_ ) )
    {
        geometry::Point2f point( barycenter_.X(), barycenter_.Y() );
        double distance = std::numeric_limits< double >::max();
        for( CIT_PointVector it = positions.begin(); it != positions.end(); ++it )
        {
            double result = barycenter_.Distance( *it );
            if( result < distance )
            {
                point = geometry::Point2f( (*it).X(), (*it).Y() );
                distance = result;
            }
        }
        barycenter_ = point;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions destructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
UrbanPositions::~UrbanPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Barycenter
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
const geometry::Point2f& UrbanPositions::Barycenter() const
{
    return barycenter_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::BoundingBox
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
const geometry::Rectangle2f& UrbanPositions::BoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsInside
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
bool UrbanPositions::IsInside( const geometry::Point2f& point ) const
{
    return polygon_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsInside
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool UrbanPositions::IsInside( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( barycenter_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
        tools.DrawDecoratedPolygon( polygon_, color_, hasInfrastructure_ ? name_ : std::string(), 13, 0, selected_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Vertices
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
const std::vector< geometry::Point2f >& UrbanPositions::Vertices() const
{
    return polygon_.Vertices();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::SetSelection
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void UrbanPositions::SetSelection( bool selected )
{
    selected_ = selected;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsSelected
// Created: JSR 2012-05-29
// -----------------------------------------------------------------------------
bool UrbanPositions::IsSelected() const
{
    return selected_;
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
