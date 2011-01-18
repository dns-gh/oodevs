// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#pragma warning( push, 0 )
#include <boost/geometry/geometry.hpp>
#pragma warning( pop )

namespace bg = boost::geometry;

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
    std::vector< bg::polygon< bg::point_xy< T > > > polygonResult;
    bg::intersection_inserter< bg::polygon< bg::point_xy< T > > >( poly2, poly1, std::back_inserter( polygonResult ) );
    double intersectArea = 0;
    for( std::vector< bg::polygon< bg::point_xy< T > > >::const_iterator it = polygonResult.begin(); it != polygonResult.end(); ++it  )
        intersectArea += area( *it );
    return static_cast< T >( intersectArea );
}
