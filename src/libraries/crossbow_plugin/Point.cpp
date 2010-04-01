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
#include "protocol/simulationsenders.h"

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
crossbow::Point::Point( const Common::MsgCoordLatLong& coord )
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
// Name: Point::Extract
// Created: JCR 2010-02-26
// -----------------------------------------------------------------------------
OGRPoint* crossbow::Point::Extract( OGRSpatialReference* spatialReference ) const
{
    OGRPoint* point = new OGRPoint( x_, y_ );
    point->assignSpatialReference( spatialReference );
    return point;
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
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Point::Serialize( Common::MsgCoordLatLong& message ) const
{
    message.set_longitude( x_ );
    message.set_latitude( y_ );
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Point::Serialize( Common::MsgLocation& message ) const
{
    message.set_type( Common::MsgLocation_Geometry_point );
    message.mutable_coordinates()->mutable_elem( 0 )->set_latitude( y_ );
    message.mutable_coordinates()->mutable_elem( 0 )->set_longitude( x_ );
    Serialize( *message.mutable_coordinates()->mutable_elem( 0 ) );
}
