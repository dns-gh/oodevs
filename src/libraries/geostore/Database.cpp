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
#include "DatabaseException.h"
#include "GeoTable.h"
#include "LogTable.h"
#include "ProjectionTable.h"
#include "terrain/TerrainFileReader.h"

using namespace geostore;

namespace bfs = boost::filesystem;
namespace blc = boost::locale::conv;

// -----------------------------------------------------------------------------
// Name: Database constructor
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
Database::Database( const bfs::path& path )
    : path_( path )
    , db_  ( 0 )
{
    bfs::path file( path_ / "Graphics" / "geostore.sqlite" );
    spatialite_init( 0 );

    int err = sqlite3_open_v2(
            blc::utf_to_utf< char >( file.wstring() ).c_str(),
            &db_,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
            NULL
        );

    if( SQLITE_OK == err )
    {
        ProjectionTable projTable( db_ ); // Just used to initialize the projection table
        logTable_.reset( new LogTable( db_ ) );
    }
    else
    {
        sqlite3_close( db_ );
        db_ = 0;
        throw DatabaseException( err, "Could not open Sqlite DB: " + file.string() );
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

void Database::LoadLayers( PointProjector_ABC& projector )
{
    bfs::path graphicsDirectory( path_ / "Graphics" );
    for( bfs::directory_iterator it( graphicsDirectory ); it != bfs::directory_iterator(); ++it )
    {
        std::string layerName = bfs::basename( *it );
        if( bfs::extension( *it ) == ".bin" && layerName != "preview" )
        {
            std::time_t accessTime;
            logTable_->GetLastAccessTime( layerName, accessTime );
            if( bfs::last_write_time( *it ) > accessTime )
            {
                // Update the table for this layer, it was modified.
                LoadLayer( layerName, *it, projector );
            }
            else
            {
                // The table was already loaded, just read it from the SQLite store.
                LoadLayer( layerName );
            }
        }
    }
}

void Database::LoadLayer( const std::string& name, const boost::filesystem::path& path, PointProjector_ABC& projector )
{
    // Create the table
    std::auto_ptr< GeoTable > table( new GeoTable( db_, name ) );
    table->FillTable( path, projector );

    // Store it in the map
    tables_[ name ] = table.release();

    // Save the last modified date for the imported data.
    logTable_->SetLastAccessTime( name, bfs::last_write_time( path ) );
}

void Database::LoadLayer( const std::string& name )
{
    // Create the table
    std::auto_ptr< GeoTable > table( new GeoTable( db_, name ) );
    table->LoadTable();

    // Store in the map
    tables_[ name ] = table.release();
}

void Database::GetTables( std::vector< GeoTable* >& tables ) const
{
    tables.clear();
    tables.reserve( tables_.size() );
    for( CIT_Tables it = tables_.begin(); it != tables_.end(); ++it )
    {
        tables.push_back( it->second );
    }
}
