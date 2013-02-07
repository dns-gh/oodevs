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
#include "LogTable.h"
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
        if( SQLITE_OK !=
            sqlite3_open_v2(
                blc::utf_to_utf< char >( path.wstring() ).c_str(),
                &db,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                0 ) )
            throw MASA_EXCEPTION_SQLITE( "Failed to initialize database" );
        sqlite3_enable_load_extension( db, 1 );
        return db;
    }
}

Database::Database( const bfs::path& dbFile, const bfs::path& layersDir,
        PointProjector_ABC& projector )
    : db_  ( Open( dbFile ), &sqlite3_close )
    , log_ ( new LogTable( db_.get() ))
{
    if( sqlite3_exec( db_.get(), "SELECT load_extension('spatialite.dll')", 0, 0, 0 ) != SQLITE_OK )
        throw std::runtime_error( "cannot find spatialite DLL to load as sqlite extension" );
    ProjectionTable projection( db_.get() ); // Just used to initialize the projection table
    LoadLayers( projector, layersDir );
}

Database::~Database()
{
    // NOTHING
}

const T_GeoTables& Database::GetTables() const
{
    return tables_;
}

void Database::LoadLayers( PointProjector_ABC& projector, const bfs::path& layersDir )
{
    for( bfs::directory_iterator it( layersDir ); it != bfs::directory_iterator(); ++it )
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
    log_->GetLastAccessTime( layer, time );
    if( bfs::last_write_time( file ) > time )
    {
        table->FillTable( file, projector );
        log_->SetLastAccessTime( layer, bfs::last_write_time( file ) );
    }
    else
        table->LoadTable();
    return table.release();
}

void Database::AddLayer( std::string layer, int geomType,
            const std::vector< TerrainObject* >& features )
{
    std::auto_ptr< GeoTable > table( new GeoTable( db_.get(), layer ));
    table->FillTable( geomType, features );
    auto other = tables_.insert( layer, table.get() );
    table.release();
    if( !other.second )
        delete other.first->second;
}
