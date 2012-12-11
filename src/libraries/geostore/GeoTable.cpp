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
        throw MASA_EXCEPTION( "Invalid geometry type." );
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
        throw MASA_EXCEPTION( "Invalid geometry name." );
    }
}

// -----------------------------------------------------------------------------
// Name: GeoTable::CreateStructure
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
void GeoTable::CreateStructure()
{
    std::stringstream query;
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
        throw MASA_EXCEPTION( "Could not load table " + GetName() );
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
    std::stringstream query;
    query << "SELECT AddGeometryColumn( '" << GetName() << "', 'geom', 4326, '";
    switch( geom_type )
    {
    case Point:
        query << "POINT";
        geometryType_ = Point;
        break;
    case Polygon:
        query << "POLYGON";
        geometryType_ = Polygon;
        break;
    case LineString:
        query << "LINESTRING";
        geometryType_ = LineString;
        break;
    default:
        throw MASA_EXCEPTION( "Invalid geometry type." );
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
    gaiaPolygonPtr polyg;
    gaiaRingPtr ring;
    geometry::Point2d firstPoint;
    int numVertices = static_cast< int >( shape.GetCoordinates().size() );
    int numRings = static_cast< int >( shape.GetSubPrimitives().size() );
    polyg = gaiaAddPolygonToGeomColl( geo, numVertices + 1, numRings );
    ring = polyg->Exterior;
    for( int i = 0; i < numVertices; ++i )
    {
        gaiaSetPoint( ring->Coords, i, shape.GetCoordinates()[ i ].X(), shape.GetCoordinates()[ i ].Y() );
        if( i == 0 )
            firstPoint.Set( shape.GetCoordinates()[ i ].X(), shape.GetCoordinates()[ i ].Y() );
    }
    gaiaSetPoint( ring->Coords, numVertices, firstPoint.X(), firstPoint.Y() );    
    if( numRings == 0 )
        return;
    geodata::Primitive_ABC::T_Vector innerRing = shape.GetSubPrimitives();
    int numParts = 0;
    for( geodata::Primitive_ABC::CIT_Vector it = innerRing.begin(); it != innerRing.end(); ++it )
    {
        numVertices = static_cast< int >( ( *it )->GetCoordinates().size() );
        if( numVertices < 3 )
            continue;
        ring = gaiaAddInteriorRing( polyg, numParts, numVertices + 1 );
        for( int i = 0; i < numVertices; ++i )
        {
            gaiaSetPoint( ring->Coords, i, ( *it )->GetCoordinates()[ i ].X(), ( *it )->GetCoordinates()[ i ].Y() );
            if( i == 0 )
                firstPoint.Set( ( *it )->GetCoordinates()[ i ].X(), ( *it )->GetCoordinates()[ i ].Y() );
        }
        gaiaSetPoint( ring->Coords, numVertices, firstPoint.X(), firstPoint.Y() );
        ++numParts;
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
    std::stringstream query;
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

    std::stringstream query;
    query << "REPLACE INTO " << GetName() << "( id, name, geom ) VALUES ( ?, ?, ? )";
    sqlite3_stmt* stmt = CreateStatement( query.str() );

    // ID
    int i = 0;

    for( auto it = features.begin(); it != features.end(); ++it )
    {
        const size_t verticesCount = ( *it )->GetCoordinates().size();
        if( ( verticesCount == 1 ) || ( geometryType_ == Polygon && verticesCount < 3 ) )
            continue;

        gaiaGeomCollPtr geo = CreateGeometry( **it );
        if( ! gaiaIsValid( geo ) )
        {
            // Skip invalid geometry
            std::cout << GetName() << " invalid geometry for :" << i << std::endl;
            // Avoid leaking this...
            gaiaFreeGeomColl( geo );
            continue;
        }

        // Increment ID
        ++i;

        // Create a blob for the geomColl
        unsigned char* blob;
        int blob_size;
        gaiaToSpatiaLiteBlobWkb( geo, &blob, &blob_size );
        // Not needed anymore, everything is in the blob!
        gaiaFreeGeomColl( geo );

        sqlite3_reset( stmt );
        sqlite3_clear_bindings( stmt );

        // ID
        sqlite3_bind_int( stmt, 1, i );
        // NAME
        std::string text = ( *it )->GetText();
        sqlite3_bind_text( stmt, 2, text.c_str(), static_cast< int >( text.length() ), SQLITE_TRANSIENT );
        // Spatialite BLOB
        sqlite3_bind_blob( stmt, 3, blob, blob_size, &free ); // Will be freed!

        // Step the query
        StepStatement( stmt );
    }

    // Free all the resources for that statement
    sqlite3_finalize( stmt );

    // Commit transaction
    ExecuteQuery( "COMMIT" );

    // Create spatial index
    MbrSpatialIndex();

    // Analyze of the table
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
        throw MASA_EXCEPTION( "Invalid geometry type." );
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

    /*std::string mbrStr = boost::lexical_cast< std::string >( poly->MinX ) + ", "
        + boost::lexical_cast< std::string >( poly->MinY ) + ", "
        + boost::lexical_cast< std::string >( poly->MaxX  ) + ", "
        + boost::lexical_cast< std::string >( poly->MaxY );*/

    std::ostringstream query;
    query << "SELECT GUnion( geom ) FROM " << GetName() << " WHERE MbrIntersects( geom, BuildMbr( ?, ?, ?, ? ) );";

    sqlite3_stmt* stmt = CreateStatement( query.str() );
    sqlite3_reset( stmt );
    sqlite3_clear_bindings( stmt );

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
        gaiaGeomCollPtr temp = gaiaFromSpatiaLiteBlobWkb( blob, blobSize );

        // Clip with the polygon
        result = gaiaGeometryIntersection( temp, poly );

        // Do not leak the temporary retrieved data
        gaiaFreeGeomColl( temp );
    }

    sqlite3_finalize( stmt );
    return result;
}
