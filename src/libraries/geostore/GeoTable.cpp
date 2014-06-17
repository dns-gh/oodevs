// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "GeoTable.h"
#include <terrain/TerrainObject.h>

using namespace geostore;

namespace
{
    GeometryType Convert( const std::string& type )
    {
        // see geometry_type et https://www.gaia-gis.it/fossil/libspatialite/wiki?name=switching-to-4.0
        switch( boost::lexical_cast< int >( type ) )
        {
            case 1:
                return Point;
            case 2:
                return LineString;
            case 3:
                return Polygon;
            default:
                throw MASA_EXCEPTION( "invalid geometry type" );
        }
    }
}

GeoTable::GeoTable( sqlite3* db, const std::string& name )
    : Table( db, name )
{
    Table::T_ResultSet results = ExecuteQuery( "SELECT geometry_type FROM geometry_columns WHERE f_table_name='" + GetName() + "'" );
    if( results.empty() )
        throw MASA_EXCEPTION( "Could not load table " + GetName() );
    SetGeometryType( Convert( results.back().back() ) );
}

GeoTable::GeoTable( sqlite3* db, const std::string& name, GeometryType geomType,
        const std::vector< boost::shared_ptr< TerrainObject > >& features )
    : Table( db, name )
{
    ExecuteQuery(
        "CREATE TABLE IF NOT EXISTS " + GetName()
        + " ( id INTEGER NOT NULL PRIMARY KEY, name VARCHAR(256) )" );
    AddGeometryColumn( geomType );
    Fill( features );
}

void GeoTable::SetGeometryType( GeometryType geomType )
{
    geometryType_ = geomType;
}

geostore::GeometryType GeoTable::GetGeometryType() const
{
    return geometryType_;
}

void GeoTable::AddGeometryColumn( GeometryType geomType )
{
    std::stringstream query;
    query << "SELECT AddGeometryColumn( '" << GetName() << "', 'geom', 4326, '";
    switch( geomType )
    {
    case Point:
        query << "POINT";
        break;
    case Polygon:
        query << "POLYGON";
        break;
    case LineString:
        query << "LINESTRING";
        break;
    default:
        throw MASA_EXCEPTION( "invalid geometry type" );
    }
    query << "', 2 )";
    ExecuteQuery( query.str() );
    SetGeometryType( geomType );
}

namespace
{
    GeometryCollection InitGeomCollPtr()
    {
        GeometryCollection ptr = gaiaAllocGeomColl();
        ptr->Srid = 4326;
        return ptr;
    }

    void AddPolygon( const geodata::Primitive_ABC& shape, std::vector< GeometryCollection >& result )
    {
        int numVertices = static_cast< int >( shape.GetCoordinates().size() );
        if( numVertices < 3 )
            return;
        GeometryCollection currentGeomCollPtr = InitGeomCollPtr();
        result.push_back( currentGeomCollPtr );
        gaiaRingPtr ring;
        gaiaPolygonPtr polyg = gaiaAddPolygonToGeomColl( currentGeomCollPtr, numVertices + 1, 0 );
        ring = polyg->Exterior; 
        for( int i = 0; i < numVertices; ++i )
            gaiaSetPoint( ring->Coords, i, shape.GetCoordinates()[ i ].X(), shape.GetCoordinates()[ i ].Y() );
        gaiaSetPoint( ring->Coords, numVertices, shape.GetCoordinates()[ 0 ].X(), shape.GetCoordinates()[ 0 ].Y() );
    }
}

std::vector< GeometryCollection > GeoTable::CreatePolygonGeometry( const TerrainObject& shape )
{
    std::vector< GeometryCollection > result;
    if( shape.GetCoordinates().size() < 3 )
        return result;
    AddPolygon( shape, result );
    geodata::Primitive_ABC::T_Vector innerRing = shape.GetSubPrimitives();
    for( auto it = innerRing.begin(); it != innerRing.end(); ++it )
        AddPolygon( **it, result );
    return result;
}

std::vector< GeometryCollection > GeoTable::CreateLineGeometry( const TerrainObject& shape )
{
    std::vector< GeometryCollection > result;
    result.push_back( InitGeomCollPtr() );
    int numVertices = static_cast< int >( shape.GetCoordinates().size() );
    gaiaLinestringPtr line = gaiaAddLinestringToGeomColl( result[0], numVertices );
    for( int i = 0; i < numVertices; ++i )
        gaiaSetPoint( line->Coords, i, shape.GetCoordinates()[ i ].X(), shape.GetCoordinates()[ i ].Y() );
    return result;
}

void GeoTable::Fill( const std::vector< boost::shared_ptr< TerrainObject > >& features )
{
    ExecuteQuery( "BEGIN" );
    sqlite3_stmt* stmt = CreateStatement( "REPLACE INTO " + GetName() + "( id, name, geom ) VALUES ( ?, ?, ? )" );
    int i = 0;
    int counter = 0;
    for( auto it = features.begin(); it != features.end(); ++it, ++counter )
    {
        std::vector< GeometryCollection > geo = CreateGeometry( **it );
        for( auto geoIt = geo.begin(); geoIt != geo.end(); ++geoIt, ++i )
        {
            if( !gaiaIsValid( *geoIt ) )
                continue;

            // Create a blob for the geomColl
            unsigned char* blob;
            int blob_size;
            gaiaToSpatiaLiteBlobWkb( *geoIt, &blob, &blob_size );
            // Not needed anymore, everything is in the blob!

            sqlite3_reset( stmt );
            sqlite3_clear_bindings( stmt );

            // ID
            sqlite3_bind_int( stmt, 1, i );
            // NAME
            const std::string text = (*it)->GetText();
            sqlite3_bind_text( stmt, 2, text.c_str(), static_cast< int >( text.length() ), SQLITE_TRANSIENT );
            // Spatialite BLOB
            sqlite3_bind_blob( stmt, 3, blob, blob_size, &gaiaFree );

            // Step the query
            StepStatement( stmt );
        }
    }
    sqlite3_finalize( stmt );
    ExecuteQuery( "COMMIT" );
    ExecuteQuery( "SELECT CreateMbrCache( '" + GetName() + "', 'geom' )" );
    ExecuteQuery( "ANALYZE " + GetName() );
}

std::vector< GeometryCollection > GeoTable::CreateGeometry( const TerrainObject& shape )
{
    switch( geometryType_ )
    {
        case Point:
            return std::vector< GeometryCollection >();
        case Polygon:
            return CreatePolygonGeometry( shape );
        case LineString:
            return CreateLineGeometry( shape );
        default:
            throw MASA_EXCEPTION( __FUNCTION__ );
    }
}

GeometryCollection GeoTable::GetFeaturesIntersectingWith( GeometryCollection poly ) const
{
    GeometryCollection result = 0;
    sqlite3_stmt* stmt = CreateStatement( "SELECT GUnion( geom ) FROM " + GetName() + " WHERE MbrIntersects( geom, BuildMbr( ?, ?, ?, ? ) );" );
    sqlite3_bind_double( stmt, 1, poly->MinX );
    sqlite3_bind_double( stmt, 2, poly->MinY );
    sqlite3_bind_double( stmt, 3, poly->MaxX );
    sqlite3_bind_double( stmt, 4, poly->MaxY );
    if( StepStatement( stmt ) == SQLITE_ROW )
    {
        // Retrieve the data...
        const unsigned char* blob = static_cast< const unsigned char * >( sqlite3_column_blob( stmt, 0 ) );
        unsigned int blobSize = sqlite3_column_bytes( stmt, 0 );
        // Create the geometry...
        GeometryCollection temp = gaiaFromSpatiaLiteBlobWkb( blob, blobSize );
        // Clip with the polygon
        result = gaiaGeometryIntersection( temp, poly );
    }
    sqlite3_finalize( stmt );
    return result;
}
