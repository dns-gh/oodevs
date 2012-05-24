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
#include "GlTools_ABC.h"
#include "Hierarchies.h"
#include "UrbanColor_ABC.h"
#include "UrbanObject_ABC.h"
#include "Viewport_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( EUrbanLevel level, const UrbanObject_ABC& object, std::vector< geometry::Point2f > positions )
    : level_            ( level )
    , object_           ( object )
    , selected_         ( false )
    , hasInfrastructure_( false )
{
    ComputeCachedValues( positions );
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
// Name: UrbanPositions::IsInside
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool UrbanPositions::IsInside( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( barycenter_ );
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
// Name: UrbanPositions::SetSelection
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void UrbanPositions::SetSelection( bool selected )
{
    selected_ = selected;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !viewport.IsHotpointVisible() )
        return;
    std::string name;
    unsigned int nameSize = 13;
    float zoom = tools.Zoom();
    if( level_ == eUrbanLevelBlock)
    {
        if( hasInfrastructure_ && zoom >  0.00045f )
        {
            name = object_.GetName().ascii();
            if( zoom <= 0.0007f )
                nameSize = static_cast< unsigned int >( nameSize * zoom * 1600 );
        }
        tools.DrawDecoratedPolygon( polygon_, object_.Get< kernel::UrbanColor_ABC >(), name, nameSize, 0, selected_ );
    }
    else
    {
        if( polygon_.IsEmpty() )
        {
            std::vector< geometry::Point2f > points;
            ComputeConvexHull( points );
            const_cast< UrbanPositions* >( this )->ComputeCachedValues( points );
        }
        if( level_ == eUrbanLevelDistrict )
        {
            if( zoom > 0.00015f && zoom <= 0.00045f )
            {
                name = object_.GetName().ascii();
                if( zoom <= 0.0002f )
                    nameSize = static_cast< unsigned int >( nameSize * zoom * 5600 );
                else if( zoom <= 0.0003f )
                    nameSize = static_cast< unsigned int >( nameSize * zoom * 5100 );
                else if( zoom <= 0.00045f )
                    nameSize = static_cast< unsigned int >( nameSize * zoom * 4300 );
            }
            tools.DrawConvexDecoratedPolygon( polygon_, object_.Get< kernel::UrbanColor_ABC >(), name, nameSize, selected_ );
        }
        else // EUrbanLevelCity
        {
            if( zoom <= 0.00015f )
            {
                name = object_.GetName().ascii();
                nameSize = static_cast< unsigned int >( nameSize * zoom * 11300 );
            }
            tools.DrawConvexDecoratedPolygon( polygon_, object_.Get< kernel::UrbanColor_ABC >(), name, nameSize, selected_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ComputeCachedValues
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanPositions::ComputeCachedValues( std::vector< geometry::Point2f >& points )
{
    if( !points.empty() )
    {
        if( points.front() == points.back() )
            points.pop_back();
        polygon_ = geometry::Polygon2f( points );
        boundingBox_ = polygon_.BoundingBox();
        barycenter_ = polygon_.Barycenter();
        area_ = polygon_.ComputeArea();
    }
}

namespace
{
    template< typename IT >
    bool FindOuterPoint( IT begin, IT end, const geometry::Point2f& from, const geometry::Vector2f& direction, geometry::Point2f& worst )
    {
        bool bFound = false;
        float rMaxProjection = 0;
        for( IT it = begin; it != end; ++it )
        {
            const geometry::Vector2f v( from, *it );
            const float rProjection = direction.CrossProduct( v );
            if( rProjection < -1.f ) // epsilon
            {
                bFound = true;
                if( rMaxProjection > rProjection )
                {
                    rMaxProjection = rProjection;
                    worst = *it;
                }
            }
        }
        return bFound;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ComputeConvexHull
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
void UrbanPositions::ComputeConvexHull( std::vector< geometry::Point2f >& points ) const
{
    std::vector< geometry::Point2f > vertices;
    const kernel::Hierarchies& hierarchies = object_.Get< kernel::Hierarchies >();
    tools::Iterator< const kernel::Entity_ABC& > it = hierarchies.CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        const std::vector< geometry::Point2f >& pos = child.Get< kernel::UrbanPositions_ABC >().Vertices();
        vertices.insert( vertices.end(), pos.begin(), pos.end() );
    }
    if( !vertices.empty() )
    {
        geometry::Point2f maxLeft = *vertices.begin();
        geometry::Point2f maxRight = maxLeft;

        for( CIT_PointVector it = vertices.begin(); it != vertices.end(); ++it )
        {
            if( it->X() < maxLeft.X() )
                maxLeft = *it;
            if( it->X() > maxRight.X() )
                maxRight = *it;
        }
        points.push_back( maxLeft );
        points.push_back( maxRight );
        unsigned int nPoint = 0;
        while( nPoint != points.size() )
        {
            unsigned int nFollowingPoint = ( nPoint + 1 ) % points.size();
            geometry::Vector2f direction( points[ nPoint ], points[ nFollowingPoint ] );
            direction.Normalize();
            geometry::Point2f worst;
            if( FindOuterPoint( vertices.begin(), vertices.end(), points[ nPoint ], direction, worst ) )
            {
                points.insert( points.begin() + nFollowingPoint, worst );
                nPoint = 0;
            }
            else
                ++nPoint;
        }
    }
}
