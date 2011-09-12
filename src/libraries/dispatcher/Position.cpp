// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Position.h"
#include "protocol/SimulationSenders.h"
#include <boost/bind.hpp>

namespace dispatcher
{
    geometry::Point2f ToPoint( const Position& position )
    {
        return geometry::Point2f( position.at( 0 ), position.at( 1 ) );
    }

    std::vector< geometry::Point2f > ToPoints( const Zone& zone )
    {
        std::vector< geometry::Point2f > result( zone.size() );
        std::transform( zone.begin(), zone.end(), result.begin(), &ToPoint );
        return result;
    }

    geometry::Polygon2f ToPolygon( const Zone& zone )
    {
        return geometry::Polygon2f( ToPoints( zone ) );
    }

    Position ToPosition( const geometry::Point2f& point )
    {
        const float* p = (const float*)( &point );
        return Position( p, p+2 );
    }

    Zone ToZone( const std::vector< geometry::Point2f >& points )
    {
        Zone result( points.size() );
        std::transform( points.begin(), points.end(), result.begin(), &ToPosition );
        return result;
    }

    Zone ToZone( const std::vector< sword::CoordLatLong >& coordinates, const kernel::CoordinateConverter_ABC& converter )
    {
        Zone result( coordinates.size() );
        std::transform( coordinates.begin(), coordinates.end(), result.begin(),
            boost::bind( &ToPosition, boost::bind( &kernel::CoordinateConverter_ABC::ConvertToXY< sword::CoordLatLong >, &converter, _1 ) ) );
        return result;
    }

}
