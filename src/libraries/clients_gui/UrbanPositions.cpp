// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************_

#include "clients_gui_pch.h"
#include "UrbanPositions.h"

#include "GlTools_ABC.h"
#include "Tesselator.h"
#include "Viewport_ABC.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/UrbanObject_ABC.h"

using namespace kernel;
using namespace gui;

namespace
{
    const float epsilon = 0.0001f;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( EUrbanLevel level, const UrbanObject_ABC& object, Tesselator& tesselator )
    : level_            ( level )
    , object_           ( object )
    , tesselator_       ( tesselator )
    , selected_         ( false )
    , hasInfrastructure_( false )
    , area_             ( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions constructor
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
UrbanPositions::UrbanPositions( EUrbanLevel level, const UrbanObject_ABC& object, std::vector< geometry::Point2f > positions,
                                Tesselator& tesselator )
    : level_            ( level )
    , object_           ( object )
    , tesselator_       ( tesselator )
    , selected_         ( false )
    , hasInfrastructure_( false )
    , area_             ( 0.f )
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
// Name: UrbanPositions::Polygon
// Created: ABR 2012-06-05
// -----------------------------------------------------------------------------
const geometry::Polygon2f& UrbanPositions::Polygon() const
{
    return polygon_;
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::ChangeGeometry
// Created: JSR 2013-01-07
// -----------------------------------------------------------------------------
void UrbanPositions::ChangeGeometry( std::vector< geometry::Point2f >& points )
{
    ComputeCachedValues( points );
    if( Entity_ABC* parent = const_cast< Entity_ABC* >( object_.Get< kernel::Hierarchies >().GetSuperior() ) )
        parent->Get< UrbanPositions_ABC >().ResetConvexHull();
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
void UrbanPositions::SetInfrastructurePresent( bool present )
{
    hasInfrastructure_ =  present;
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
// Name: UrbanPositions::Draw
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
void UrbanPositions::Draw( const geometry::Point2f& /*where*/, const Viewport_ABC& viewport, GlTools_ABC& tools ) const
{
    if( !viewport.IsHotpointVisible() && ( level_ == eUrbanLevelBlock || !polygon_.IsEmpty() ) )
        return;
    if( level_ == eUrbanLevelBlock )
    {
        std::string name;
        float zoom = tools.Zoom();
        unsigned int nameSize = 13;
        if( hasInfrastructure_ && zoom >  0.00045f )
        {
            name = object_.GetName().toStdString();
            if( zoom <= 0.0007f )
                nameSize = static_cast< unsigned int >( nameSize * zoom * 1600 );
        }
        tools.DrawDecoratedPolygon( tessellatedPolygon_, polygon_.Vertices(), boundingBox_.Center(), name, nameSize );
        if( selected_ )
            tools.DrawSelectedPolygon( polygon_.Vertices() );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanPositions::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void UrbanPositions::Pick( const geometry::Point2f& where, const Viewport_ABC& viewport, GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools );
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

        EliminateRedundantVertices( points, epsilon );
        ChopSpikes( epsilon );

        boundingBox_ = polygon_.BoundingBox();
        barycenter_ = polygon_.Barycenter();
        area_ = polygon_.ComputeArea();
        tessellatedPolygon_ = tesselator_.Tessellate( polygon_ );
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
