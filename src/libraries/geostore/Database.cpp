// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "Database.h"
#include "GeoTable.h"
#include "LogTable.h"
#include "ProjectionTable.h"
#include "terrain/TerrainFileReader.h"

using namespace geostore;

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: Database constructor
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
Database::Database( const bfs::path& path )
    : path_     ( path )
    , db_       ( 0 )
    , err_msg   ( 0 )
{
    bfs::path file( path_ / "Graphics" / "geostore.sqlite" );
    spatialite_init( 0 );

    int ret = sqlite3_open_v2(
        file.string().c_str(),
        &db_,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        NULL );

    if( ret == SQLITE_OK )
    {
        // Initialize the projection table in the database
        ProjectionTable projTable( db_ );

        logTable_.reset( new LogTable( db_ ) );
    }
    else
    {
        sqlite3_close( db_ );
        db_ = 0;
        throw std::runtime_error( "Could not open Sqlite DB: " + file.string() );
    }
}

// -----------------------------------------------------------------------------
// Name: Database destructor
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
Database::~Database()
{
    for( IT_Tables it = tables_.begin(); it != tables_.end(); ++it )
        delete it->second;
    sqlite3_close( db_ );
}

// -----------------------------------------------------------------------------
// Name: Database::CreateTable
// Created: AME 2010-07-20
// -----------------------------------------------------------------------------
void Database::CreateTable( const TerrainFileReader& file )
{
    GeoTable* table = new GeoTable( db_, file.name_ );
    table->AddGeometryColumn( file.geomType_ );
    table->Fill( file.features_ );
    tables_[ file.name_ ] = table;
}

// -----------------------------------------------------------------------------
// Name: Database::GetTable
// Created: AME 2010-07-28
// -----------------------------------------------------------------------------
void Database::GetTable( const std::string& name )
{
    GeoTable* table = new GeoTable( db_, name );
    char** results;
    int n_rows;
    int n_columns;
    std::string sqlRequest = "SELECT type FROM geometry_columns WHERE f_table_name='" + name + "';";
    sqlite3_get_table( db_, sqlRequest.c_str(), &results, &n_rows, &n_columns, &err_msg );
    if( n_rows == 1 )
        table->SetGeometry( results[ 1 ] );
    tables_[ name ] = table;
    sqlite3_free_table( results );
}

// -----------------------------------------------------------------------------
// Name: Database::HasTable
// Created: AME 2010-07-28
// -----------------------------------------------------------------------------
bool Database::HasTable( const std::string& name )
{
    bool tableExist = false;
    char** results;
    int n_rows;
    int n_columns;
    std::string sqlRequest = "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = '" + name + "';"; 
    sqlite3_get_table( db_, sqlRequest.c_str(), &results, &n_rows, &n_columns, &err_msg );
    if( atoi ( results[ 1 ] ) != 1 )
        tableExist = true;
    sqlite3_free_table( results );
    return tableExist;
}
