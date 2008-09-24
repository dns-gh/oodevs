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

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
plugins::crossbow::Point::Point()
    : x_( 0 )
    , y_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
plugins::crossbow::Point::Point( const ASN1T_CoordLatLong& coord )
    : x_( coord.longitude )
    , y_( coord.latitude )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2007-11-06
// -----------------------------------------------------------------------------
plugins::crossbow::Point::Point( IGeometryPtr geometry )
{
    IPointPtr point;
    if( SUCCEEDED( geometry.QueryInterface( IID_IPoint, &point ) ) )
        point->QueryCoords( &x_, &y_ );
}

// -----------------------------------------------------------------------------
// Name: Point destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
plugins::crossbow::Point::~Point()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point::Accept
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void plugins::crossbow::Point::Accept( ShapeVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Point::UpdateGeometry
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void plugins::crossbow::Point::UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
{
    if( geometry == NULL )
        geometry.CreateInstance( CLSID_Point );
    geometry->putref_SpatialReference( spatialReference );

    IZAwarePtr zAwareness;
    geometry.QueryInterface( IID_IZAware, &zAwareness );
    zAwareness->put_ZAware( VARIANT_TRUE );

    IPointPtr point;
    geometry.QueryInterface( IID_IPoint, &point );
    point->PutCoords( x_, y_ );
    point->put_Z( 0 );
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void plugins::crossbow::Point::Serialize( ASN1T_CoordLatLong& asn ) const
{
    asn.latitude = y_;
    asn.longitude = x_;
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void plugins::crossbow::Point::Serialize( ASN1T_Location& asn ) const
{
    asn.type = EnumLocationType::point;
    asn.coordinates.n = 1;
    asn.coordinates.elem = new ASN1T_CoordLatLong[1];
    asn.coordinates.elem[0].latitude = y_;
    asn.coordinates.elem[0].longitude = x_;
}
