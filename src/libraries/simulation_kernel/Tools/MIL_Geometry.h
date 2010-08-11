// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Geometry_h_
#define __MIL_Geometry_h_

#include <geometry/Types.h>
#pragma warning( push )
#pragma warning( disable : 4127 4100 4244 )
#include <boost/geometry/geometry.hpp>
#pragma warning( pop )

namespace bg = boost::geometry;

// =============================================================================
/** @class  MIL_Geometry
    @brief  MIL Geometry
*/
// Created: SLG 2010-04-30
// =============================================================================
class MIL_Geometry : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Geometry();
    virtual ~MIL_Geometry();
    //@}

    //! @name Operations
    //@{
    template< typename T > static geometry::Polygon2< T > Scale( geometry::Polygon2< T > polygon, T distance );
    template< typename T > static geometry::Polygon2< T > ComputeHull( geometry::Polygon2< T > polygon );
    template< typename T > static T IntersectionArea( const geometry::Polygon2< T > polygon1, const geometry::Polygon2< T > polygon2 );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_Geometry( const MIL_Geometry& );            //!< Copy constructor
    MIL_Geometry& operator=( const MIL_Geometry& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename T > static bool FindOuterPoint( const std::vector< geometry::Point2< T > > vertices, const geometry::Point2< T >& from, const geometry::Vector2< T >& direction, geometry::Point2< T >& worst );
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::Scale
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
template< typename T >
geometry::Polygon2< T > MIL_Geometry::Scale( geometry::Polygon2< T > polygon, T distance )
{
    geometry::Polygon2< T > hull   = ComputeHull( polygon );
    geometry::Point2< T > barycenter = hull.Barycenter();
    const std::vector< geometry::Point2< T > > vertices = hull.Vertices();
    geometry::Polygon2< T > result;
    for( std::vector< geometry::Point2< T > >::const_iterator it = vertices.begin(); it != vertices.end(); ++it )
    {
        geometry::Vector2< T > scaleVector( *it, barycenter );
        geometry::Vector2< T > scaleVector2 = scaleVector.Normalized();
        result.Add( barycenter + scaleVector + scaleVector2 * distance );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::ComputeHull
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
template< typename T >
geometry::Polygon2< T > MIL_Geometry::ComputeHull( geometry::Polygon2< T > polygon )
{
    const std::vector< geometry::Point2< T > > vertices = polygon.Vertices();
    geometry::Point2< T > maxLeft = *( vertices.begin() );
    geometry::Point2< T > maxRight = *( vertices.begin() );
    for( std::vector< geometry::Point2< T > >::const_iterator it = vertices.begin(); it != vertices.end() ; ++it )
    {
        if( it->X() < maxLeft.X() )
            maxLeft = *it;
        if( it->X() > maxRight.X() )
            maxRight = *it;
    }
    std::vector< geometry::Point2< T > > hull;
    hull.resize( 0 );
    hull.push_back( maxLeft );
    hull.push_back( maxRight );
    unsigned int nPoint = 0;
    while( nPoint != hull.size() )
    {
        unsigned int nFollowingPoint = ( nPoint + 1 ) % hull.size();
        geometry::Vector2< T > direction( hull[ nPoint ], hull[ nFollowingPoint ] );
        direction.Normalize();
        geometry::Point2< T > worst;
        if( FindOuterPoint( vertices, hull[ nPoint ], direction, worst ) )
        {
            hull.insert( hull.begin() + nFollowingPoint, worst );
            nPoint = 0;
        }
        else
            ++nPoint;
    }
    return geometry::Polygon2< T >( hull );
}

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::ComputeHull
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
template< typename T >
bool MIL_Geometry::FindOuterPoint( const std::vector< geometry::Point2< T > > vertices, const geometry::Point2< T >& from, const geometry::Vector2< T >& direction, geometry::Point2< T >& worst )
{
    bool bFound = false;
    float rMaxProjection = 0;
    for( std::vector< geometry::Point2< T > >::const_iterator it = vertices.begin(); it != vertices.end(); ++it )
    {
        const geometry::Vector2< T > v( from, *it );
        const float rProjection = direction.CrossProduct( v );
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

// -----------------------------------------------------------------------------
// Name: MIL_Geometry::IntersectionArea
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
template< typename T >
T MIL_Geometry::IntersectionArea( const geometry::Polygon2< T > polygon1, const geometry::Polygon2< T > polygon2 )
{
    bg::polygon< bg::point_xy< T > > poly1;
    bg::polygon< bg::point_xy< T > > poly2;
    {
        geometry::Polygon2< T >::T_Vertices vertices1 = polygon1.Vertices();
        std::vector< bg::point_xy< T > > vectorTemp;
        for( geometry::Polygon2< T >::CIT_Vertices it = vertices1.begin(); it != vertices1.end(); ++it )
        {
            bg::point_xy< T > p( it->X(), it->Y() );
            vectorTemp.push_back( p );
        }
        bg::assign( poly1, vectorTemp );
        bg::correct( poly1 );
    }
    {
        geometry::Polygon2< T >::T_Vertices vertices2 = polygon2.Vertices();
        std::vector< bg::point_xy< T > > vectorTemp;
        for( geometry::Polygon2< T >::CIT_Vertices it = vertices2.begin(); it != vertices2.end(); ++it )
        {
            bg::point_xy< T > p( it->X(), it->Y() );
            vectorTemp.push_back( p );
        }
        bg::assign( poly2, vectorTemp );
        bg::correct( poly2 );
    }
    std::vector< bg::polygon< bg::point_xy< T > > > polygonResult;
    bg::intersection_inserter< bg::polygon< bg::point_xy< T > > >( poly2, poly1, std::back_inserter( polygonResult ) );
    T intersectArea = 0;
    for( std::vector< bg::polygon< bg::point_xy< T > > >::const_iterator it = polygonResult.begin(); it != polygonResult.end(); ++it  )
        intersectArea += float( area( *it ) );
    return intersectArea;
}

#endif // __MIL_Geometry_h_
