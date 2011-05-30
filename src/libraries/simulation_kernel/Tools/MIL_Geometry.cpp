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
#pragma warning( push, 0 )
#include <boost/geometry/geometry.hpp>
#pragma warning( pop )

namespace bg = boost::geometry;

namespace
{
    bool FindOuterPoint( const T_PointVector& vertices, const MT_Vector2D& from, const MT_Vector2D& direction, MT_Vector2D& worst )
    {
        bool bFound = false;
        double rMaxProjection = 0;
        for( CIT_PointVector it = vertices.begin(); it != vertices.end(); ++it )
        {
            const double rProjection = CrossProduct( direction, MT_Vector2D( *it - from ) );
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
void MIL_Geometry::Scale( TER_Polygon& result, const T_PointVector& polygon, double distance )
{
    T_PointVector ret;
    T_PointVector hull;
    ComputeHull( hull, polygon );
    MT_Vector2D barycenter = MT_ComputeBarycenter( hull );
    for( CIT_PointVector it = hull.begin(); it != hull.end(); ++it )
    {
        MT_Vector2D scaleVector( barycenter - *it );
        ret.push_back( barycenter + scaleVector + scaleVector.Normalized() * distance );
    }
    result.Reset( ret );
}

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::ComputeHull
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
void MIL_Geometry::ComputeHull( T_PointVector& hull, const T_PointVector& vertices )
{
    if( vertices.empty() )
        return;
    CIT_PointVector maxLeft = vertices.begin();
    CIT_PointVector maxRight = vertices.begin();
    for( CIT_PointVector it = vertices.begin(); it != vertices.end() ; ++it )
    {
        if( it->rX_ < maxLeft->rX_ )
            maxLeft = it;
        if( it->rX_ > maxRight->rX_ )
            maxRight = it;
    }
    hull.push_back( *maxLeft );
    hull.push_back( *maxRight );
    unsigned int nPoint = 0;
    MT_Vector2D worst;
    while( nPoint != hull.size() )
    {
        unsigned int nFollowingPoint = ( nPoint + 1 ) % hull.size();
        MT_Vector2D direction( hull[ nFollowingPoint ] - hull[ nPoint ] );
        direction.Normalize();
        if( FindOuterPoint( vertices, hull[ nPoint ], direction, worst ) )
        {
            hull.insert( hull.begin() + nFollowingPoint, worst );
            nPoint = 0;
        }
        else
            ++nPoint;
    }
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
