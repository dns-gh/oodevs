// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Position_h_
#define __Position_h_

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "geometry/Types.h"
#include "game_asn/generated/base.h"
#include <vector>

namespace dispatcher
{

    typedef std::vector< float >    Position;
    typedef std::vector< Position > Zone;

    geometry::Point2f ToPoint( const Position& position );
    std::vector< geometry::Point2f > ToPoints( const Zone& zone );
    geometry::Polygon2f ToPolygon( const Zone& zone );

    Position ToPosition( const geometry::Point2f& point );
    Zone ToZone( const std::vector< geometry::Point2f >& points );

    Zone ToZone( const std::vector< ASN1T_CoordLatLong >& coordinates, const kernel::CoordinateConverter_ABC& converter );
}

#endif // __Position_h_

