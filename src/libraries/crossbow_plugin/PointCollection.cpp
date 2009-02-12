// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "esri_plugin_pch.h"
#include "PointCollection.h"
#include "Point.h"

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: PointCollection constructor
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
esri::PointCollection::PointCollection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointCollection constructor
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
esri::PointCollection::PointCollection( const ASN1T_CoordLatLongList& asn )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        points_.push_back( esri::Point( asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: PointCollection constructor
// Created: JCR 2007-11-06
// -----------------------------------------------------------------------------
esri::PointCollection::PointCollection( IGeometryPtr geometry )
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
// Name: PointCollection destructor
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
esri::PointCollection::~PointCollection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointCollection::Accept
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
void esri::PointCollection::Accept( ShapeVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: PointCollection::UpdateGeometry
// Created: JCR 2007-08-31
// -----------------------------------------------------------------------------
void esri::PointCollection::UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
{
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
// Name: PointCollection::Serialize
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
void esri::PointCollection::Serialize( ASN1T_Location& asn ) const
{
    asn.coordinates.n = points_.size();
    asn.coordinates.elem = new ASN1T_CoordLatLong[asn.coordinates.n];
    int i = 0;
    for( CIT_Points it = points_.begin(); it != points_.end(); ++it )
        it->Serialize( asn.coordinates.elem[i++] );
}
