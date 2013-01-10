// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "GeometryFactory.h"
#include "terrain/PointProjector_ABC.h"

using namespace geostore;

// -----------------------------------------------------------------------------
// Name: GeometryFactory constructor
// Created: AME 2010-07-30
// -----------------------------------------------------------------------------
GeometryFactory::GeometryFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GeometryFactory destructor
// Created: AME 2010-07-30
// -----------------------------------------------------------------------------
GeometryFactory::~GeometryFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GeometryFactory::CreatePolygonGeometry
// Created: AME 2010-07-30
// -----------------------------------------------------------------------------
gaiaGeomCollPtr GeometryFactory::CreatePolygonGeometry(const geometry::Polygon2f& footPrint, PointProjector_ABC& projector )
{
    gaiaGeomCollPtr poly = InitGeometryCollection();
    gaiaPolygonPtr polyg = gaiaAddPolygonToGeomColl( poly, static_cast< int >( footPrint.Vertices().size() + 1 ), 0 );
    FillPolygon( footPrint, projector, polyg );
    gaiaMbrGeometry( poly );
    return poly;
}

// -----------------------------------------------------------------------------
// Name: GeometryFactory::GetGeometryCollection
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
gaiaGeomCollPtr GeometryFactory::InitGeometryCollection()
{
    gaiaGeomCollPtr geom = gaiaAllocGeomColl();
    geom->Srid = 4326; // Set the Spatial Reference ID !!
    return geom;
}

// -----------------------------------------------------------------------------
// Name: GeometryFactory::CheckValidity
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
gaiaGeomCollPtr GeometryFactory::Validate( gaiaGeomCollPtr geom )
{
    if( geom && ( gaiaDimension( geom ) < 0 || gaiaIsValid( geom ) == 0 ) )
    {
        gaiaFreeGeomColl( geom );
        geom = 0;
    }
    return geom;
}

// -----------------------------------------------------------------------------
// Name: GeometryFactory::AddPolygonGeometryToCollection
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void GeometryFactory::AddPolygonGeometryToCollection( const geometry::Polygon2f& footPrint, PointProjector_ABC& projector, gaiaGeomCollPtr& geomColl )
{
    geomColl = Validate( geomColl );
    if( geomColl )
    {
        gaiaPolygonPtr polyg = gaiaAddPolygonToGeomColl( geomColl, static_cast< int >( footPrint.Vertices().size() + 1 ), 0 );
        FillPolygon( footPrint, projector, polyg );
        gaiaMbrGeometry( geomColl );
    }
    else
        geomColl = CreatePolygonGeometry( footPrint, projector );
}

// -----------------------------------------------------------------------------
// Name: GeometryFactory::FillPolygon
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void GeometryFactory::FillPolygon( const geometry::Polygon2f& footPrint, PointProjector_ABC& projector, gaiaPolygonPtr& polyg ) const
{
    gaiaRingPtr ring = polyg->Exterior;
    double rLatitudeInDegrees = 0;
    double rLongitudeInDegrees = 0;
    const int numVertices = static_cast< int >( footPrint.Vertices().size() );
    for( int i = numVertices - 1; i >= 0; --i )
    {
        const geometry::Point2f& p = footPrint.Vertices()[ i ];
        geometry::Point2d point( p.X(), p.Y() );
        projector.Unproject( point, rLatitudeInDegrees, rLongitudeInDegrees );
        gaiaSetPoint( ring->Coords, i, rLongitudeInDegrees, rLatitudeInDegrees );
    }
    gaiaSetPoint( ring->Coords, numVertices, rLongitudeInDegrees, rLatitudeInDegrees ); // same as i == 0
    gaiaMbrPolygon( polyg );
}
