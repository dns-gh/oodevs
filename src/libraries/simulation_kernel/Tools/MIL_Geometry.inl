// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "MT_Tools/Mt_Vector2DTypes.h"
#include "simulation_terrain/TER_Localisation.h"

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::IntersectionArea
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
template< typename T >
T MIL_Geometry::IntersectionArea( const geometry::Polygon2< T >& polygon1, const geometry::Polygon2< T >& polygon2 )
{
    bg::polygon< bg::point_xy< T > > poly1;
    bg::polygon< bg::point_xy< T > > poly2;
    {
        const geometry::Polygon2< T >::T_Vertices& vertices1 = polygon1.Vertices();
        std::vector< bg::point_xy< T > > vectorTemp;
        for( geometry::Polygon2< T >::CIT_Vertices it = vertices1.begin(); it != vertices1.end(); ++it )
            vectorTemp.push_back( bg::point_xy< T >( it->X(), it->Y() ) );
        bg::assign( poly1, vectorTemp );
        bg::correct( poly1 );
    }
    {
        const geometry::Polygon2< T >::T_Vertices& vertices2 = polygon2.Vertices();
        std::vector< bg::point_xy< T > > vectorTemp;
        for( geometry::Polygon2< T >::CIT_Vertices it = vertices2.begin(); it != vertices2.end(); ++it )
            vectorTemp.push_back( bg::point_xy< T >( it->X(), it->Y() ) );
        bg::assign( poly2, vectorTemp );
        bg::correct( poly2 );
    }
    return PrivateIntersectionArea( poly1, poly2 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::IntersectionArea
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
inline
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
// Name: MIL_Geometry::PrivateIntersectionArea
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
template< typename T >
inline
T MIL_Geometry::PrivateIntersectionArea( const bg::polygon< bg::point_xy< T > >& poly1, const bg::polygon< bg::point_xy< T > >& poly2 )
{
    std::vector< bg::polygon< bg::point_xy< T > > > polygonResult;
    bg::intersection_inserter< bg::polygon< bg::point_xy< T > > >( poly2, poly1, std::back_inserter( polygonResult ) );
    double intersectArea = 0;
    for( std::vector< bg::polygon< bg::point_xy< T > > >::const_iterator it = polygonResult.begin(); it != polygonResult.end(); ++it  )
        intersectArea += area( *it );
    return static_cast< T >( intersectArea );
}
