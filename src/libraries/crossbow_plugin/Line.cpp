// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Line.h"
#include "Point.h"

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
crossbow::Line::Line()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Line::Line( const ASN1T_CoordUTMList& asn )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        points_.push_back( crossbow::Point( asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: SBO 2007-11-06
// -----------------------------------------------------------------------------
crossbow::Line::Line( IGeometryPtr geometry )
{
    IPointCollectionPtr points;
    if( SUCCEEDED( geometry.QueryInterface( IID_IPointCollection, &points ) ) )
    {
        long count = 0;
        points->get_PointCount( &count );
        for( long i = 0; i < count; ++i )
        {
            IPointPtr point;
            points->get_Point( i, &point );
            points_.push_back( Point( point ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Line destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Line::~Line()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line::Accept
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::Line::Accept( ShapeVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Line::UpdateGeometry
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void crossbow::Line::UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
{
    if( geometry == NULL )
        geometry.CreateInstance( CLSID_Polyline );
    geometry->putref_SpatialReference( spatialReference );

    IZAwarePtr zAwareness;
    geometry.QueryInterface( IID_IZAware, &zAwareness );
    zAwareness->put_ZAware( VARIANT_TRUE );

    IPointCollectionPtr points;
    geometry.QueryInterface( IID_IPointCollection, &points );
    for( CIT_Points it = points_.begin(); it != points_.end(); ++it )
    {
        IPointPtr point( CLSID_Point );
        it->UpdateGeometry( point, spatialReference );
        points->AddPoint( point );
    }
}

// -----------------------------------------------------------------------------
// Name: Line::Serialize
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Line::Serialize( ASN1T_Location& asn ) const
{
    throw std::runtime_error( "Line serialization not implemented" ); // $$$$ SBO 2007-09-26: 
}
