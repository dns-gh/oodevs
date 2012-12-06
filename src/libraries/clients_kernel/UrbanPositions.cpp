// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UrbanPositions.h"
#include "GlTools_ABC.h"
#include "Hierarchies.h"
#include "UrbanColor_ABC.h"
#include "UrbanObject_ABC.h"
#include "Viewport_ABC.h"

using namespace kernel;

float UrbanPositions::epsilon_ = 0.0001f;

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( EUrbanLevel level, const UrbanObject_ABC& object )
    : level_            ( level )
    , object_           ( object )
    , selected_         ( false )
    , hasInfrastructure_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: ABR 2012-06-05
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
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
UrbanPositions::~UrbanPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::SetInfrastructurePresent
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
void UrbanPositions::SetInfrastructurePresent()
{
    hasInfrastructure_ = true;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::SetSelection
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
void UrbanPositions::SetSelection( bool selected )
{
    selected_ = selected;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsSelected
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
bool UrbanPositions::IsSelected() const
{
    return selected_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Barycenter
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
const geometry::Point2f& UrbanPositions::Barycenter() const
{
    return barycenter_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::BoundingBox
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
const geometry::Rectangle2f& UrbanPositions::BoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: std::vector< geometry::Point2f >& UrbanPositions::Vertices
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
const std::vector< geometry::Point2f >& UrbanPositions::Vertices() const
{
    return polygon_.Vertices();
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Polygon
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
const geometry::Polygon2f& UrbanPositions::Polygon() const
{
    return polygon_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsInside
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
bool UrbanPositions::IsInside( const geometry::Point2f& point ) const
{
    return polygon_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::IsInside
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
bool UrbanPositions::IsInside( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( barycenter_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ComputeArea
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
float UrbanPositions::ComputeArea() const
{
    return area_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Draw
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !viewport.IsHotpointVisible() && ( level_ == eUrbanLevelBlock || !polygon_.IsEmpty() ) )
        return;
    std::string name;
    unsigned int nameSize = 13;
    float zoom = tools.Zoom();
    if( level_ == eUrbanLevelBlock)
    {
        if( hasInfrastructure_ && zoom >  0.00045f )
        {
            name = object_.GetName().toAscii().constData();
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
            if( Entity_ABC* parent = const_cast< Entity_ABC* >( object_.Get< kernel::Hierarchies >().GetSuperior() ) )
                parent->Get< UrbanPositions_ABC >().ResetConvexHull();
        }
        if( level_ == eUrbanLevelDistrict )
        {
            if( zoom > 0.00015f && zoom <= 0.00045f )
            {
                name = object_.GetName().toAscii().constData();
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
                name = object_.GetName().toAscii().constData();
                nameSize = static_cast< unsigned int >( nameSize * zoom * 11300 );
            }
            tools.DrawConvexDecoratedPolygon( polygon_, object_.Get< kernel::UrbanColor_ABC >(), name, nameSize, selected_ );
        }
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
// Name: UrbanPositions::ResetConvexHull
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanPositions::ResetConvexHull()
{
    if( level_ != eUrbanLevelBlock )
        polygon_ = geometry::Polygon2f();
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

        for( auto it = vertices.begin(); it != vertices.end(); ++it )
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

        EliminateRedundantVertices( points, epsilon_ );
        ChopSpikes( epsilon_ );

        boundingBox_ = polygon_.BoundingBox();
        barycenter_ = polygon_.Barycenter();
        area_ = polygon_.ComputeArea(); 
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::EliminateRedundantVertices
// Created: CMA 2011-12-15
// -----------------------------------------------------------------------------
void UrbanPositions::EliminateRedundantVertices( const T_PointVector& vertices, float epsilon )
{
    if( vertices.size() < 4 )
    {
        polygon_ = geometry::Polygon2f( vertices );
        return;
    }

    T_PointVector result;
    unsigned int first  = 0;
    unsigned int second = 1;
    unsigned int third  = 2;
    while( third <= vertices.size() )
    {
        geometry::Segment2f segment( vertices[first], vertices[third % vertices.size()] );
        if( segment.SquareDistance( vertices[second] ) <= epsilon )
            second = third++;
        else
        {
            result.push_back( vertices[second] );
            first = second;
            second = third++;
        }
    }

    if( result.empty() )
    {
        polygon_ = geometry::Polygon2f( vertices );
        return;
    }

    geometry::Segment2f segment( result.front(), result.back() );
    if( segment.SquareDistance( vertices[0] ) >= epsilon )
        result.push_back( vertices[0] );
    polygon_ = geometry::Polygon2f( result );
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ChopSpikes
// Created: CMA 2011-12-15
// -----------------------------------------------------------------------------
void UrbanPositions::ChopSpikes( float epsilon )
{
    std::set< geometry::Point2f > spikeTips;
    for( geometry::Polygon2f::CIT_Edges it = polygon_.Edges_begin(); it != polygon_.Edges_end(); ++it )
    {
        geometry::Polygon2f::CIT_Edges it1 = it;
        geometry::Polygon2f::CIT_Edges it2 = it;
        ++it2;
        if( it2 == polygon_.Edges_end() )
            it2 = polygon_.Edges_begin();

        geometry::Segment2f segment1( *it1 );
        geometry::Segment2f segment2( *it2 );
        if( ( segment1.SquareDistance( segment2.End() ) <= epsilon ) || ( segment2.SquareDistance( segment1.Start() ) <= epsilon ) )
            spikeTips.insert( segment1.End() );
    }

    std::vector< geometry::Point2f > vertices;
    for( geometry::Polygon2f::CIT_Vertices it = polygon_.Vertices().begin(); it != polygon_.Vertices().end(); ++it )
    {
        geometry::Point2f point( *it );
        if( spikeTips.find( point ) == spikeTips.end() )
            vertices.push_back( point );
    }
    polygon_ = geometry::Polygon2f( vertices );
}
