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
