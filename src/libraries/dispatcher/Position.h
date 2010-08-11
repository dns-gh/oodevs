// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Position_h_
#define __Position_h_

#include "clients_kernel/CoordinateConverter_ABC.h"
#include <geometry/Types.h>
#include <vector>

namespace Common
{
    class MsgCoordLatLong;
}

namespace dispatcher
{

    typedef std::vector< float >    Position;
    typedef std::vector< Position > Zone;

    geometry::Point2f ToPoint( const Position& position );
    std::vector< geometry::Point2f > ToPoints( const Zone& zone );
    geometry::Polygon2f ToPolygon( const Zone& zone );

    Position ToPosition( const geometry::Point2f& point );
    Zone ToZone( const std::vector< geometry::Point2f >& points );

    Zone ToZone( const std::vector< Common::MsgCoordLatLong >& coordinates, const kernel::CoordinateConverter_ABC& converter );
}

#endif // __Position_h_

