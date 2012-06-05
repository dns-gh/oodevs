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
    geom->Srid = 4326;
    return geom;
}

// -----------------------------------------------------------------------------
// Name: GeometryFactory::CheckValidity
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
bool GeometryFactory::CheckValidity( gaiaGeomCollPtr& geom )
{
    if( !geom || gaiaDimension( geom ) == -1 || !gaiaIsValid( geom ) )
        return false;
    else
        return true;
}

// -----------------------------------------------------------------------------
// Name: GeometryFactory::AddPolygonGeometryToCollection
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void GeometryFactory::AddPolygonGeometryToCollection( const geometry::Polygon2f& footPrint, PointProjector_ABC& projector, gaiaGeomCollPtr& geomColl )
{
    if( !CheckValidity( geomColl ) )
        geomColl = CreatePolygonGeometry( footPrint, projector );
    else
    {
        gaiaPolygonPtr polyg = gaiaAddPolygonToGeomColl( geomColl, static_cast< int >( footPrint.Vertices().size() + 1 ), 0 );
        FillPolygon( footPrint, projector, polyg );
        gaiaMbrGeometry( geomColl );
    }
}

// -----------------------------------------------------------------------------
// Name: GeometryFactory::FillPolygon
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void GeometryFactory::FillPolygon( const geometry::Polygon2f& footPrint, PointProjector_ABC& projector, gaiaPolygonPtr& polyg )
{
    gaiaRingPtr ring = polyg->Exterior;    
    double rLatitudeInDegrees;
    double rLongitudeInDegrees;
    int count = 0;
    int numVertices = static_cast< int >( footPrint.Vertices().size() );
    for( geometry::Polygon2f::CIT_Vertices it = footPrint.Vertices().begin(); it != footPrint.Vertices().end(); ++it )
    {
        geometry::Point2d point( ( *it ).X(), ( *it ).Y() );
        projector.Unproject( point, rLatitudeInDegrees, rLongitudeInDegrees );
        gaiaSetPoint( ring->Coords, count, rLongitudeInDegrees, rLatitudeInDegrees );
        if( it == footPrint.Vertices().begin() )
            gaiaSetPoint( ring->Coords, numVertices, rLongitudeInDegrees, rLatitudeInDegrees );
        ++count;
    }
    gaiaMbrPolygon( polyg );
}
