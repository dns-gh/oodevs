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
crossbow::Point::Point( const ASN1T_CoordLatLong& coord )
    : x_( 0 )
    , y_( 0 )
{
    SetFromUTM( std::string( (const char*)coord.data, 15 ) );
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
// Name: Point::SetFromUTM
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::Point::SetFromUTM( const std::string& input )
{
    IPointPtr   pointWGS, point;
    BSTR        dms, utm, mgrs;
    ICoordinateToolPtr converter( CLSID_CoordinateTool );
    if( !FAILED( converter->ConvertLocation( CComVariant( input.c_str() ), 4, CComVariant( L"WGS 1984 (WGS84)" ), CComVariant( L"WGS 1984 (WGS84)" ), &pointWGS, &point, &dms, &utm, &mgrs ) ) )
        point->QueryCoords( &x_, &y_ );
}

// -----------------------------------------------------------------------------
// Name: Point::Accept
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::Point::Accept( ShapeVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Point::UpdateGeometry
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void crossbow::Point::UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
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

namespace
{
    std::string ToMgrs( double x, double y )
    {
        IPointPtr point( CLSID_Point );
        point->PutCoords( x, y );

        IPointPtr   wgs, datum;
        BSTR        dms, utm, mgrs;
        ICoordinateToolPtr converter( CLSID_CoordinateTool ); //WGS 1984 (WGS84)
        if ( FAILED( converter->ConvertLocation( CComVariant( (IPoint*)point ), 1, CComVariant( L"WGS 1984 (WGS84)" ), CComVariant( L"WGS 1984 (WGS84)" ), &wgs, &datum, &dms, &utm, &mgrs ) ) )
            throw std::runtime_error( "Conversion failed" );
        return _com_util::ConvertBSTRToString( mgrs );
    }
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Point::Serialize( ASN1T_Location& asn ) const
{
    asn.type = EnumLocationType::point;
    asn.coordinates.n = 1;
    asn.coordinates.elem = new ASN1T_CoordLatLong[1];
    asn.coordinates.elem[0] = ToMgrs( x_, y_ ).c_str();
}
