// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Point.h"
#include "protocol/SimulationSenders.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#pragma warning( push, 0 )
#include <gdal/ogr_feature.h>
#pragma warning( pop )

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
crossbow::Point::Point()
    : x_( 0 )
    , y_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Point::Point( const sword::CoordLatLong& coord )
    : x_( coord.longitude() )
    , y_( coord.latitude() )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Point::Point( const OGRPoint& point )
    : x_( point.getX() )
    , y_( point.getY() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Point::~Point()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: JCR 2010-04-08
// -----------------------------------------------------------------------------
void crossbow::Point::Serialize( OGRFeature& feature, OGRSpatialReference* spatialReference ) const
{
    OGRPoint point( x_, y_ );

    point.assignSpatialReference( spatialReference );
    feature.SetGeometry( &point );
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: JCR 2009-04-27
// -----------------------------------------------------------------------------
void crossbow::Point::Serialize( std::ostream& geometry ) const
{
    const int srid = 0;
    std::stringstream ss;
    SerializeCoordinates( ss, ',' );
    geometry << "st_point("
                << ss.str() << ',' << srid
             << ")";
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: JCR 2010-04-08
// -----------------------------------------------------------------------------
void crossbow::Point::Serialize( OGRPoint& point, OGRSpatialReference* spatialReference ) const
{
    point.setX( x_ );
    point.setY( y_ );
    point.assignSpatialReference( spatialReference );
}

// -----------------------------------------------------------------------------
// Name: Point::SerializeCoordinates
// Created: JCR 2009-04-27
// -----------------------------------------------------------------------------
std::ostream& crossbow::Point::SerializeCoordinates( std::ostream& geometry, char sep ) const
{
    geometry << x_ << sep << y_;
    return geometry;
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: JCR 2010-12-04
// -----------------------------------------------------------------------------
void crossbow::Point::Serialize( kernel::Location_ABC& location, const kernel::CoordinateConverter_ABC& converter ) const
{
    location.AddPoint( converter.ConvertFromGeo( geometry::Point2d( x_, y_ ) ) );
}
