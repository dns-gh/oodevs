// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Geometry.h"
#include "MT_Tools/Mt_Vector2DTypes.h"
#include "simulation_terrain/TER_Localisation.h"
#include <boost/geometry/geometry.hpp>

namespace bg = boost::geometry;

namespace
{
    bool FindOuterPoint( const geometry::Polygon2f::T_Vertices& vertices, const geometry::Point2f& from, const geometry::Vector2f& direction, geometry::Point2f& worst )
    {
        bool bFound = false;
        float rMaxProjection = 0;
        for( geometry::Polygon2f::T_Vertices::const_iterator it = vertices.begin(); it != vertices.end(); ++it )
        {
            const float rProjection = direction.CrossProduct( geometry::Vector2f( from, *it ) );
            if( rProjection < -1 ) // epsilon
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
// Name: MIL_Geometry::Scale
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
void MIL_Geometry::Scale( geometry::Polygon2f& result, const geometry::Polygon2f& polygon, float distance )
{
    geometry::Polygon2f hull;
    ComputeHull( hull, polygon );
    geometry::Point2f barycenter = hull.Barycenter();
    const std::vector< geometry::Point2f >& vertices = hull.Vertices();
    for( std::vector< geometry::Point2f >::const_iterator it = vertices.begin(); it != vertices.end(); ++it )
    {
        geometry::Vector2f scaleVector( *it, barycenter );
        result.Add( barycenter + scaleVector + scaleVector.Normalized() * distance );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::ComputeHull
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
void MIL_Geometry::ComputeHull( geometry::Polygon2f& result, const geometry::Polygon2f& polygon )
{
    const geometry::Polygon2f::T_Vertices& vertices = polygon.Vertices();
    std::vector< geometry::Point2f >::const_iterator maxLeft = vertices.begin();
    std::vector< geometry::Point2f >::const_iterator maxRight = vertices.begin();
    for( std::vector< geometry::Point2f >::const_iterator it = vertices.begin(); it != vertices.end() ; ++it )
    {
        if( it->X() < maxLeft->X() )
            maxLeft = it;
        if( it->X() > maxRight->X() )
            maxRight = it;
    }
    std::vector< geometry::Point2f > hull;
    hull.push_back( *maxLeft );
    hull.push_back( *maxRight );
    unsigned int nPoint = 0;
    geometry::Point2f worst;
    while( nPoint != hull.size() )
    {
        unsigned int nFollowingPoint = ( nPoint + 1 ) % hull.size();
        geometry::Vector2f direction( hull[ nPoint ], hull[ nFollowingPoint ] );
        direction.Normalize();
        if( FindOuterPoint( vertices, hull[ nPoint ], direction, worst ) )
        {
            hull.insert( hull.begin() + nFollowingPoint, worst );
            nPoint = 0;
        }
        else
            ++nPoint;
    }
    result = geometry::Polygon2f( hull );
}

namespace
{
    template< typename T >
    T PrivateIntersectionArea( const bg::polygon< bg::point_xy< T > >& poly1, const bg::polygon< bg::point_xy< T > >& poly2 )
    {
        std::vector< bg::polygon< bg::point_xy< T > > > polygonResult;
        bg::intersection_inserter< bg::polygon< bg::point_xy< T > > >( poly2, poly1, std::back_inserter( polygonResult ) );
        double intersectArea = 0;
        for( std::vector< bg::polygon< bg::point_xy< T > > >::const_iterator it = polygonResult.begin(); it != polygonResult.end(); ++it  )
            intersectArea += area( *it );
        return static_cast< T >( intersectArea );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::IntersectionArea
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
double MIL_Geometry::IntersectionArea( const TER_Localisation& localisation1, const TER_Localisation& localisation2 )
{
    bg::polygon< bg::point_xy< double > > poly1;
    bg::polygon< bg::point_xy< double > > poly2;
    {
        CT_PointVector& vertices1 = localisation1.GetPoints();
        std::vector< bg::point_xy< double > > vectorTemp;
        for( CIT_PointVector it = vertices1.begin(); it != vertices1.end(); ++it )
            vectorTemp.push_back( bg::point_xy< double >( it->rX_, it->rY_ ) );
        bg::assign( poly1, vectorTemp );
        bg::correct( poly1 );
    }
    {
        CT_PointVector& vertices2 = localisation2.GetPoints();
        std::vector< bg::point_xy< double > > vectorTemp;
        for( CIT_PointVector it = vertices2.begin(); it != vertices2.end(); ++it )
            vectorTemp.push_back( bg::point_xy< double >( it->rX_, it->rY_ ) );
        bg::assign( poly2, vectorTemp );
        bg::correct( poly2 );
    }
    return PrivateIntersectionArea( poly1, poly2 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::IntersectionArea
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
float MIL_Geometry::IntersectionArea( const geometry::Polygon2f& polygon1, const geometry::Polygon2f& polygon2 )
{
    bg::polygon< bg::point_xy< float > > poly1;
    bg::polygon< bg::point_xy< float > > poly2;
    {
        const geometry::Polygon2f::T_Vertices& vertices1 = polygon1.Vertices();
        std::vector< bg::point_xy< float > > vectorTemp;
        for( geometry::Polygon2f::CIT_Vertices it = vertices1.begin(); it != vertices1.end(); ++it )
            vectorTemp.push_back( bg::point_xy< float >( it->X(), it->Y() ) );
        bg::assign( poly1, vectorTemp );
        bg::correct( poly1 );
    }
    {
        const geometry::Polygon2f::T_Vertices& vertices2 = polygon2.Vertices();
        std::vector< bg::point_xy< float > > vectorTemp;
        for( geometry::Polygon2f::CIT_Vertices it = vertices2.begin(); it != vertices2.end(); ++it )
            vectorTemp.push_back( bg::point_xy< float >( it->X(), it->Y() ) );
        bg::assign( poly2, vectorTemp );
        bg::correct( poly2 );
    }
    return PrivateIntersectionArea( poly1, poly2 );
}
