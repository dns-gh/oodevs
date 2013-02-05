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
#include "ProjectionTable.h"
#include "GeoTable.h"
#pragma warning ( push, 0 )
#include <boost/locale.hpp>
#pragma warning ( pop )

using namespace geostore;

namespace bfs = boost::filesystem;
namespace blc = boost::locale::conv;

namespace
{
    sqlite3* Open( const bfs::path& path )
    {
        sqlite3* db = 0;
        spatialite_init( 0 );
        const bfs::path file( path / "Graphics" / "geostore.sqlite" );
        if( SQLITE_OK !=
            sqlite3_open_v2(
                blc::utf_to_utf< char >( file.wstring() ).c_str(),
                &db,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                0 ) )
            throw MASA_EXCEPTION_SQLITE( "Failed to initialize database" );
        return db;
    }
}

Database::Database( const bfs::path& path, PointProjector_ABC& projector )
    : path_( path )
    , db_  ( Open( path ), &sqlite3_close )
    , log_ ( db_.get() )
{
    ProjectionTable projection( db_.get() ); // Just used to initialize the projection table
    LoadLayers( projector );
}

Database::~Database()
{
    // NOTHING
}

void Database::LoadLayers( PointProjector_ABC& projector )
{
    for( bfs::directory_iterator it( path_ / "Graphics" ); it != bfs::directory_iterator(); ++it )
    {
        std::string layer = bfs::basename( *it );
        if( bfs::extension( *it ) == ".bin" && layer != "preview" )
            tables_.insert( layer, LoadLayer( projector, *it, layer ) );
    }
}

GeoTable* Database::LoadLayer( PointProjector_ABC& projector, const bfs::path& file, const std::string& layer )
{
    std::auto_ptr< GeoTable > table( new GeoTable( db_.get(), layer ) );
    std::time_t time;
    log_.GetLastAccessTime( layer, time );
    if( bfs::last_write_time( file ) > time )
    {
        table->FillTable( file, projector );
        log_.SetLastAccessTime( layer, bfs::last_write_time( file ) );
    }
    else
        table->LoadTable();
    return table.release();
}
