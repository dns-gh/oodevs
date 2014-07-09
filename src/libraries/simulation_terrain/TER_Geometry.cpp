// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_Geometry.h"
#include "TER_Localisation.h"
#include "MT_Tools/Mt_Vector2DTypes.h"
#pragma warning( push, 0 )
#pragma warning( disable: 4702 )
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#pragma warning( pop )

namespace bg = boost::geometry;

// -----------------------------------------------------------------------------
// Name: TER_Geometry::Scale
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
void TER_Geometry::Scale( TER_Polygon& result, const T_PointVector& polygon, double distance )
{
    T_PointVector ret;
    T_PointVector hull;
    ComputeHull( hull, polygon );
    MT_Vector2D barycenter = MT_ComputeBarycenter( hull );
    for( auto it = hull.begin(); it != hull.end(); ++it )
    {
        MT_Vector2D scaleVector( barycenter - *it );
        ret.push_back( barycenter + scaleVector + scaleVector.Normalized() * distance );
    }
    result.Reset( ret );
}

// -----------------------------------------------------------------------------
// Name: TER_Geometry::ComputeHull
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
void TER_Geometry::ComputeHull( T_PointVector& hull, const T_PointVector& vertices )
{
    if( vertices.empty() )
        return;
    if( vertices.size() < 3 )
    {
        hull = vertices;
        return;
    }
    TER_Polygon polygon;
    polygon.Reset( vertices, true );
    hull = polygon.GetBorderPoints();
}

namespace
{
    typedef bg::model::polygon< bg::model::d2::point_xy< double > > bgPolygon;

    double PrivateIntersectionArea( const bgPolygon& poly1, const bgPolygon& poly2 )
    {
        std::vector< bgPolygon > polygonResult;
        bg::intersection( poly2, poly1, polygonResult );
        double intersectArea = 0;
        for( auto it = polygonResult.begin(); it != polygonResult.end(); ++it )
            intersectArea += bg::area( *it );
        return static_cast< double >( intersectArea );
    }

    void GetPolygonFromLocalisation( const T_PointVector& vertices, bgPolygon& poly )
    {
        std::vector< bg::model::d2::point_xy< double > > vectorTemp; vectorTemp.reserve( vertices.size() );
        for( auto it = vertices.begin(); it != vertices.end(); ++it )
            vectorTemp.push_back( bg::model::d2::point_xy< double >( it->rX_, it->rY_ ) );
        bg::assign_points( poly, vectorTemp );
        bg::correct( poly );
    }

    void GetPolygonFromLocalisation( const TER_Localisation& localisation, bgPolygon& poly )
    {
        GetPolygonFromLocalisation( localisation.GetPoints(), poly );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Geometry::IntersectionArea
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
double TER_Geometry::IntersectionArea( const TER_Localisation& localisation1, const TER_Localisation& localisation2 )
{
    bgPolygon poly1, poly2;
    GetPolygonFromLocalisation( localisation1, poly1 );
    GetPolygonFromLocalisation( localisation2, poly2 );
    return PrivateIntersectionArea( poly1, poly2 );
}

// -----------------------------------------------------------------------------
// Name: TER_Geometry::IsEntirelyCovered
// Created: MMC 2013-06-11
// -----------------------------------------------------------------------------
bool TER_Geometry::IsEntirelyCovered( const TER_Localisation& toCover, const std::vector< TER_Localisation >& covers )
{
    if( covers.empty() )
        return false;

    bgPolygon polyToCover;
    GetPolygonFromLocalisation( toCover, polyToCover );
    
    std::list< bgPolygon > notCovered;
    notCovered.push_back( polyToCover );
    for( auto itCover = covers.begin(); itCover != covers.end(); ++itCover )
    {
        bgPolygon cover;
        GetPolygonFromLocalisation( *itCover, cover );

        std::list< bgPolygon > newNotCovered;
        for( auto it = notCovered.begin(); it != notCovered.end(); ++it )
        {
            std::list< bgPolygon > result;
            bg::difference( *it, cover, result );
            if( !result.empty() )
                newNotCovered.insert( newNotCovered.end(), result.begin(), result.end() );
        }
        notCovered = newNotCovered;
    }
    return notCovered.empty();
}
