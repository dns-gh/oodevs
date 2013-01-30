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

using namespace geostore;

// -----------------------------------------------------------------------------
// Name: GeoTable constructor
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
GeoTable::GeoTable( sqlite3* db, const std::string& name )
    : db_  ( db )
    , name_( name )
    , geo_ ( 0 )
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

// -----------------------------------------------------------------------------
// Name: GeoTable::GetName
// Created: AME 2010-07-21
// -----------------------------------------------------------------------------
const std::string& GeoTable::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: GeoTable::GetGeometry
// Created: AME 2010-07-22
// -----------------------------------------------------------------------------
const std::string& GeoTable::GetGeometry() const
{
    return geometry_;
}

// -----------------------------------------------------------------------------
// Name: GeoTable::CreateStructure
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
void GeoTable::CreateStructure()
{
    std::string sqlRequest = "CREATE TABLE " + name_ + "(";
    sqlRequest += "id INTEGER NOT NULL PRIMARY KEY,";
    sqlRequest += "name VARCHAR(256) );";
    sqlite3_exec(db_, sqlRequest.c_str(), NULL, NULL, &err_msg );
}

// -----------------------------------------------------------------------------
// Name: GeoTable::AddGeometryColumn
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
void GeoTable::AddGeometryColumn( int geom_type )
{   
    std::string sqlRequest;
    switch( geom_type )
    {
        case 0: 
            geometry_ = "POINT";
            break;
        case 1:
            geometry_ = "POLYGON";
            break;
        case 2:
        default:
            geometry_ = "LINESTRING";
            break;
    }
    sqlRequest = "SELECT AddGeometryColumn('" + name_ + "', 'geom', 4326, '" + geometry_ + "', 2 )";
    sqlite3_exec( db_, sqlRequest.c_str(), NULL, NULL, &err_msg );
}

// -----------------------------------------------------------------------------
// Name: GeoTable::SetGeometry
// Created: AME 2010-07-28
// -----------------------------------------------------------------------------
void GeoTable::SetGeometry( const std::string& name )
{
    geometry_ = name;
}

namespace
{
    gaiaGeomCollPtr InitGeomCollPtr()
    {
        gaiaGeomCollPtr ptr = gaiaAllocGeomColl();
        ptr->Srid = 4326;
        return ptr;
    }

    void AddPolygon( const geodata::Primitive_ABC& shape, std::vector< gaiaGeomCollPtr >& result )
    {
        int numVertices = static_cast< int >( shape.GetCoordinates().size() );
        if( numVertices < 3 )
            return;
        gaiaGeomCollPtr currentGeomCollPtr = InitGeomCollPtr();
        result.push_back( currentGeomCollPtr );
        gaiaRingPtr ring;
        gaiaPolygonPtr polyg = gaiaAddPolygonToGeomColl( currentGeomCollPtr, numVertices + 1, 0 );
        ring = polyg->Exterior; 
        for( int i = 0; i < numVertices; ++i )
            gaiaSetPoint( ring->Coords, i, shape.GetCoordinates()[ i ].X(), shape.GetCoordinates()[ i ].Y() );
        gaiaSetPoint( ring->Coords, numVertices, shape.GetCoordinates()[ 0 ].X(), shape.GetCoordinates()[ 0 ].Y() );
    }
}

// -----------------------------------------------------------------------------
// Name: GeoTable::CreatePolygonGeometry
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
std::vector< gaiaGeomCollPtr > GeoTable::CreatePolygonGeometry( const TerrainObject& shape )
{
    std::vector< gaiaGeomCollPtr > result;    
    AddPolygon( shape, result );
    geodata::Primitive_ABC::T_Vector innerRing = shape.GetSubPrimitives();
    for( geodata::Primitive_ABC::CIT_Vector it = innerRing.begin(); it != innerRing.end(); ++it )
        AddPolygon( **it, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: GeoTable::CreateLineGeometry
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
std::vector< gaiaGeomCollPtr > GeoTable::CreateLineGeometry( const TerrainObject& shape )
{
    std::vector< gaiaGeomCollPtr > result;
    result.push_back( InitGeomCollPtr() );
    int numVertices = static_cast< int >( shape.GetCoordinates().size() );
    gaiaLinestringPtr line = gaiaAddLinestringToGeomColl( result[0], numVertices );
    for( int i = 0; i < numVertices; ++i )
        gaiaSetPoint( line->Coords, i, shape.GetCoordinates()[ i ].X(), shape.GetCoordinates()[ i ].Y() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: GeoTable::MbrSpatialIndex
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
void GeoTable::MbrSpatialIndex()
{
    std::string sqlRequest( "SELECT CreateMbrCache('" + name_ + "', 'geom')" );
    sqlite3_exec( db_, sqlRequest.c_str(), NULL, NULL, &err_msg );
}

// -----------------------------------------------------------------------------
// Name: GeoTable::Fill
// Created: AME 2010-07-20
// -----------------------------------------------------------------------------
void GeoTable::Fill( std::vector< TerrainObject* > features )
{
    sqlite3_stmt* stmt;
    unsigned char* blob;
    int blob_size;
    int i( 0 );
    int ret = sqlite3_exec(db_, "BEGIN", NULL, NULL, &err_msg );
    std::string sqlRequest = "INSERT INTO " + name_ + "(id, name, geom) VALUES (?, ?, ?)";
    ret = sqlite3_prepare_v2( db_, sqlRequest.c_str(), static_cast< int >( sqlRequest.size() ), &stmt, NULL );    
    std::vector< TerrainObject* >::const_iterator it;
    for( it = features.begin(); it != features.end(); ++it )
    {
        if( ( ( *it )->GetCoordinates().size() == 1 ) || ( geometry_ == "POLYGON" && ( *it )->GetCoordinates().size() < 3 ) )
            continue;
        std::vector< gaiaGeomCollPtr > geo = CreateGeometry( **it );
        for( auto geoIt = geo.begin(); geoIt != geo.end(); ++geoIt )
        {
            geo_ = *geoIt;
            ++i;            
            if( !gaiaIsValid( geo_ ) )
            {
                std::cout << name_ << " invalid geometry for :" << i << std::endl;
                gaiaFreeGeomColl( geo_ );
                continue;
            }
            gaiaToSpatiaLiteBlobWkb( geo_, &blob, &blob_size );
            gaiaFreeGeomColl( geo_ );
            sqlite3_reset( stmt );
            sqlite3_clear_bindings( stmt );
            sqlite3_bind_int64( stmt, 1, i );
            sqlite3_bind_text( stmt, 2, ( *it )->GetText().c_str(), -1, SQLITE_TRANSIENT );
            sqlite3_bind_blob(stmt, 3, blob, blob_size, free );
            ret = sqlite3_step( stmt );
            if( ret != SQLITE_DONE )
            {
                sqlite3_finalize( stmt );
                throw std::runtime_error( __FUNCTION__ );
            }
        }
    }
    sqlite3_finalize( stmt );
    ret = sqlite3_exec(db_, "COMMIT" , NULL, NULL, &err_msg );
    MbrSpatialIndex(); 
    sqlRequest = "ANALYSE " + name_ + ";";
    ret = sqlite3_exec(db_, sqlRequest.c_str() , NULL, NULL, &err_msg );
}

// -----------------------------------------------------------------------------
// Name: GeoTable::CreateGeometry
// Created: AME 2010-07-20
// -----------------------------------------------------------------------------
std::vector< gaiaGeomCollPtr > GeoTable::CreateGeometry( const TerrainObject& shape )
{
    if( geometry_ == "POLYGON" )
        return CreatePolygonGeometry( shape );
    else if( geometry_ == "LINESTRING" ) 
        return CreateLineGeometry( shape );
    else
        throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: GeoTable::GetFeaturesIntersectsWith
// Created: AME 2010-07-21
// -----------------------------------------------------------------------------
gaiaGeomCollPtr GeoTable::GetFeaturesIntersectsWith( gaiaGeomCollPtr poly )
{
    gaiaGeomCollPtr pt( 0 ); 
    gaiaGeomCollPtr temp;
    sqlite3_stmt* stmt( 0 );
    std::string mbrStr = boost::lexical_cast< std::string >( poly->MinX ) + ", " 
                         + boost::lexical_cast< std::string >( poly->MinY ) + ", " 
                         + boost::lexical_cast< std::string >( poly->MaxX  ) + ", " 
                         + boost::lexical_cast< std::string >( poly->MaxY ); 
    std::string sqlRequest = "SELECT GUnion( geom ) FROM " + name_ + " WHERE MbrIntersects( geom, BuildMbr(" + mbrStr + ") );";
    int ret = sqlite3_prepare_v2(db_, sqlRequest.c_str(), static_cast< int >( sqlRequest.size() ), &stmt, NULL );
    sqlite3_reset( stmt );
    sqlite3_clear_bindings(stmt);
    ret = sqlite3_step( stmt );
    if( ret == SQLITE_ROW )
    {
        temp = gaiaFromSpatiaLiteBlobWkb( static_cast< const unsigned char * >( sqlite3_column_blob( stmt, 0 ) ), sqlite3_column_bytes( stmt, 0 ) );       
        pt = gaiaGeometryIntersection( temp, poly );
        gaiaFreeGeomColl( temp );
    }
    sqlite3_finalize( stmt );
    return pt;
}
