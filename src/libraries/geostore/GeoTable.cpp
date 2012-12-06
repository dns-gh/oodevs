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
#include "DatabaseException.h"
#include "terrain/TerrainObject.h"
#include "terrain/TerrainFileReader.h"

using namespace geostore;

// -----------------------------------------------------------------------------
// Name: GeoTable constructor
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
GeoTable::GeoTable( sqlite3* db, const std::string& name )
    : Table( db, name )
{
    CreateStructure();
}

// -----------------------------------------------------------------------------
// Name: GeoTable destructor
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
GeoTable::~GeoTable()
{
    // NOTHING
}

GeoTable::GeometryType GeoTable::GetGeometryType() const
{
    return geometryType_;
}

/*std::string GeoTable::GetGeometry() const
{
    switch( geometryType_ )
    {
    case Polygon:
        return std::string( "POLYGON" );
    case LineString:
        return std::string( "LINESTRING" );
    default:
        throw std::runtime_error( __FUNCTION__ );
    }
}*/

void GeoTable::SetGeometry( const std::string& name )
{
    if( "POLYGON" == name )
    {
        geometryType_ = Polygon;
    }
    else if( "LINESTRING" == name )
    {
        geometryType_ = LineString;
    }
    else
    {
        throw std::runtime_error( __FUNCTION__ );
    }
}

// -----------------------------------------------------------------------------
// Name: GeoTable::CreateStructure
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
void GeoTable::CreateStructure()
{
    std::ostringstream query;
    query   << "CREATE TABLE IF NOT EXISTS " << GetName()
            << " ( id INTEGER NOT NULL PRIMARY KEY, name VARCHAR(256) )";
    ExecuteQuery( query.str() );
}

void GeoTable::LoadTable()
{
    Table::T_ResultSet results;
    ExecuteQuery( "SELECT type FROM geometry_columns WHERE f_table_name='" + GetName() + "'", results );

    if( results.empty() )
    {
        throw std::runtime_error( std::string( __FUNCTION__ ) + ": Could not load table " + GetName() );
    }

    SetGeometry( results.back().back() );
}

void GeoTable::FillTable( const boost::filesystem::path& path, PointProjector_ABC& proj )
{
    TerrainFileReader reader( path.string(), proj );
    AddGeometryColumn( reader.geomType_ );
    Fill( reader.features_ );
}

// -----------------------------------------------------------------------------
// Name: GeoTable::AddGeometryColumn
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
void GeoTable::AddGeometryColumn( int geom_type )
{
    std::ostringstream query;
    query << "SELECT AddGeometryColumn( '" << GetName() << ", 'geom', 4326, '";
    switch( geom_type )
    {
    case Point: 
        query << "POINT";
        break;
    case Polygon:
        query << "POLYGON";
        break;
    case LineString:
    default:
        query << "LINESTRING";
        break;
    }
    query << "', 2 )";

    ExecuteQuery( query.str() );
}

// -----------------------------------------------------------------------------
// Name: GeoTable::CreatePolygonGeometry
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
void GeoTable::CreatePolygonGeometry( gaiaGeomCollPtr geo, const TerrainObject& shape )
{
    auto coordinates = shape.GetCoordinates();
    int numVertices = static_cast< int >( coordinates.size() );

    auto innerRings = shape.GetSubPrimitives();

    geometry::Point2d firstPoint;

    gaiaPolygonPtr polyg = gaiaAddPolygonToGeomColl( geo, numVertices + 1, static_cast< int >( innerRings.size() ) );
    gaiaRingPtr ring = polyg->Exterior;

    if( numVertices != 0 )
    {
        for( int i = 0; i < numVertices; ++i )
        {
            gaiaSetPoint( ring->Coords, i, coordinates[ i ].X(), coordinates[ i ].Y() );
        }
        // Close the ring
        gaiaSetPoint( ring->Coords, numVertices, coordinates[ 0 ].X(), coordinates[ 0 ].Y() );
    }

    if( innerRings.empty() )
    {
        // We are done...
        return;
    }

    int ringPos = 0;
    for( auto it = innerRings.begin(); it != innerRings.end(); ++it )
    {
        coordinates = ( *it )->GetCoordinates();
        numVertices = static_cast< int >( coordinates.size() );
        if( numVertices < 3 )
        {
            continue;
        }

        ring = gaiaAddInteriorRing( polyg, ringPos, numVertices + 1 );
        for( int i = 0; i < numVertices; ++i )
        {
            gaiaSetPoint( ring->Coords, i, coordinates[ i ].X(), coordinates[ i ].Y() );
        }
        // Close the ring
        gaiaSetPoint( ring->Coords, numVertices, coordinates[ 0 ].X(), coordinates[ 0 ].Y() );
        ++ringPos;
    }
}

// -----------------------------------------------------------------------------
// Name: GeoTable::CreateLineGeometry
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
void GeoTable::CreateLineGeometry( gaiaGeomCollPtr geo, const TerrainObject& shape )
{
    const int numVertices = static_cast< int >( shape.GetCoordinates().size() );
    gaiaLinestringPtr line = gaiaAddLinestringToGeomColl( geo, numVertices );
    for( int i = 0; i < numVertices; ++i )
    {
        gaiaSetPoint( line->Coords, i, shape.GetCoordinates()[ i ].X(), shape.GetCoordinates()[ i ].Y() );
    }
}

// -----------------------------------------------------------------------------
// Name: GeoTable::MbrSpatialIndex
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
void GeoTable::MbrSpatialIndex()
{
    std::ostringstream query;
    query << "SELECT CreateMbrCache( '" << GetName() << "', 'geom' )";
    ExecuteQuery( query.str() );
}

// -----------------------------------------------------------------------------
// Name: GeoTable::Fill
// Created: AME 2010-07-20
// -----------------------------------------------------------------------------
void GeoTable::Fill( const std::vector< TerrainObject* >& features )
{
    // Begin transaction
    ExecuteQuery( "BEGIN" );

    std::ostringstream query;
    query << "INSERT INTO " << GetName() << "( id, name, geom ) VALUES ( ?, ?, ? )";
    sqlite3_stmt* stmt = CreateStatement( query.str() );

    // Index count
    int i = 0;
    int err;

    for( auto it = features.begin(); it != features.end(); ++it )
    {
        if( ( ( *it )->GetCoordinates().size() == 1 ) || ( geometryType_ == Polygon && ( *it )->GetCoordinates().size() < 3 ) )
            continue;

        // Increment index
        ++i;

        gaiaGeomCollPtr geo = CreateGeometry( **it );
        if( ! gaiaIsValid( geo ) )
        {
            // Skip invalid geometry
            std::cout << GetName() << " invalid geometry for :" << i << std::endl;
            // Avoid leaking this...
            gaiaFreeGeomColl( geo );
            continue;
        }

        // Create a blob for the geomColl
        unsigned char* blob;
        int blob_size;
        gaiaToSpatiaLiteBlobWkb( geo, &blob, &blob_size );
        // Not needed anymore, everything is in the blob!
        gaiaFreeGeomColl( geo );

        sqlite3_reset( stmt );
        sqlite3_clear_bindings( stmt );
        sqlite3_bind_int( stmt, 1, i );
        std::string text = ( *it )->GetText();
        sqlite3_bind_text( stmt, 2, text.c_str(), static_cast< int >( text.length() ), SQLITE_TRANSIENT );
        sqlite3_bind_blob( stmt, 3, blob, blob_size, &free ); // Will be freed!
        err = sqlite3_step( stmt );

        if( err == SQLITE_DONE /*|| err == SQLITE_ROW*/ )
        {
            continue;
        }
        else
        {
            sqlite3_finalize( stmt );
            throw std::runtime_error( __FUNCTION__ ); // It failed!
        }
    }

    // Free all the resources for that statement
    sqlite3_finalize( stmt );

    // Commit transaction
    ExecuteQuery( "COMMIT" );

    // Create spatial index
    MbrSpatialIndex();

    // Start analyze of the table
    ExecuteQuery( "ANALYZE " + GetName() );
}

// -----------------------------------------------------------------------------
// Name: GeoTable::CreateGeometry
// Created: AME 2010-07-20
// -----------------------------------------------------------------------------
gaiaGeomCollPtr GeoTable::CreateGeometry( const TerrainObject& shape )
{
    gaiaGeomCollPtr geo = gaiaAllocGeomColl();
    geo->Srid = 4326;

    switch( geometryType_ )
    {
    case Polygon:
        CreatePolygonGeometry( geo, shape );
        break;
    case LineString:
        CreateLineGeometry( geo, shape );
        break;
    default:
        gaiaFreeGeomColl( geo );
        throw std::runtime_error( __FUNCTION__ );
    }

    return geo;
}

// -----------------------------------------------------------------------------
// Name: GeoTable::GetFeaturesIntersectsWith
// Created: AME 2010-07-21
// -----------------------------------------------------------------------------
gaiaGeomCollPtr GeoTable::GetFeaturesIntersectingWith( gaiaGeomCollPtr poly )
{
    gaiaGeomCollPtr result = nullptr;

    sqlite3_stmt* stmt = CreateStatement( "SELECT GUnion( geom ) FROM ? WHERE MbrIntersects( geom, BuildMbr( ?, ?, ?, ? ) )" );
    sqlite3_reset( stmt );
    sqlite3_clear_bindings( stmt );
    sqlite3_bind_text( stmt, 1, GetName().c_str(), static_cast< int >( GetName().length() ), SQLITE_TRANSIENT );
    sqlite3_bind_double( stmt, 2, poly->MinX );
    sqlite3_bind_double( stmt, 3, poly->MinY );
    sqlite3_bind_double( stmt, 4, poly->MaxX );
    sqlite3_bind_double( stmt, 5, poly->MaxY );

    if( StepStatement( stmt ) == SQLITE_ROW )
    {
        // Retrieve the data...
        gaiaGeomCollPtr temp = gaiaFromSpatiaLiteBlobWkb(
                static_cast< const unsigned char * >( sqlite3_column_blob( stmt, 0 ) ),
                sqlite3_column_bytes( stmt, 0 )
            );

        // Clip with the polygon
        result = gaiaGeometryIntersection( temp, poly );

        // Do not leak the temporary retrieved data
        gaiaFreeGeomColl( temp );
    }

    sqlite3_finalize( stmt );
    return result;
}
