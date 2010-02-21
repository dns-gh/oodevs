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
// Created: SBO 2007-11-06
// -----------------------------------------------------------------------------
crossbow::Point::Point( IGeometryPtr geometry )
{
    IPointPtr point;
    if( SUCCEEDED( geometry.QueryInterface( IID_IPoint, &point ) ) )
        point->QueryCoords( &x_, &y_ );
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
// Name: Point::UpdateGeometry
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void crossbow::Point::Serialize( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
{
    if( geometry == NULL )
        geometry.CreateInstance( CLSID_Point );
    geometry->putref_SpatialReference( spatialReference );

    IZAwarePtr zAwareness;
    geometry.QueryInterface( IID_IZAware, &zAwareness );
    zAwareness->put_ZAware( VARIANT_FALSE );

    IPointPtr point;
    geometry.QueryInterface( IID_IPoint, &point );
    point->PutCoords( x_, y_ );
    point->put_Z( 0 );
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
    message.set_latitude( y_ );
    message.set_longitude( x_ );
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
}
