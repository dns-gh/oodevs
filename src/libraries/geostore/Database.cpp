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
#include <terrain/TerrainFileReader.h>
#pragma warning ( push, 0 )
#include <boost/locale.hpp>
#pragma warning ( pop )
#include <boost/filesystem.hpp>

using namespace geostore;

namespace blc = boost::locale::conv;

namespace
{
    struct SqliteInitializer
    {
        SqliteInitializer()
        {
            sqlite3_initialize();
        }

        ~SqliteInitializer()
        {
            sqlite3_shutdown();
        }
    };

    sqlite3* Open( const tools::Path& path )
    {
        static const SqliteInitializer initializer;
        sqlite3* db = 0;
        if( SQLITE_OK !=
            sqlite3_open_v2(
                blc::utf_to_utf< char >( path.ToBoost().wstring() ).c_str(),
                &db,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
                0 ) )
            throw MASA_EXCEPTION_SQLITE( "Failed to initialize database" );
        sqlite3_enable_load_extension( db, 1 );
        return db;
    }
    struct ProjectionTable : Table
    {
        ProjectionTable( sqlite3* db )
            : Table( db, "spatial_ref_sys" )
        {
            if( ExecuteQuery( "SELECT name FROM sqlite_master WHERE type='table' AND name='spatial_ref_sys'" ).empty() )
            {
                ExecuteQuery( "SELECT InitSpatialMetadata()" );
                ExecuteQuery(
                    "REPLACE INTO"
                    "   spatial_ref_sys ( srid, auth_name, auth_srid, ref_sys_name, proj4text ) "
                    "VALUES"
                    "   ( 4326, 'epgs', 4326, 'WGS 84', '+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs' )"
                    );
            }
        }
    };
}

Database::Database( const tools::Path& dbFile, const tools::Path& layersDir, PointProjector_ABC& projector )
    : db_  ( Open( dbFile ), &sqlite3_close )
    , log_ ( new LogTable( db_.get() ) )
{
    spatialite_init( 0 );
    if( sqlite3_exec( db_.get(), "SELECT load_extension('spatialite.dll')", 0, 0, 0 ) != SQLITE_OK )
        throw std::runtime_error( "cannot find spatialite DLL to load as sqlite extension" );
    ProjectionTable( db_.get() );
    LoadLayers( projector, layersDir );
}

Database::~Database()
{
    // NOTHING
}

void Database::LoadLayers( PointProjector_ABC& projector, const tools::Path& layersDir )
{
    for( auto it( layersDir.begin() ); it != layersDir.end(); ++it )
    {
        std::string layer = it->BaseName().ToUTF8();
        if( it->Extension() == ".bin" && layer != "preview" )
            LoadLayer( layer, projector, *it );
    }
}

void Database::LoadLayer( std::string layer, PointProjector_ABC& projector, const tools::Path& file )
{
    std::time_t time;
    log_->GetLastAccessTime( layer, time );
    if( file.LastWriteTime() <= time )
        tables_.insert( layer, new GeoTable( db_.get(), layer ) );
    else
    {
        TerrainFileReader reader( file, projector );
        std::vector< boost::shared_ptr< TerrainObject > > features;
        while( auto feature = reader.Next() )
            features.push_back( feature );
        AddLayer( layer, static_cast< GeometryType >( reader.GetGeomType() ), features );
        log_->SetLastAccessTime( layer, file.LastWriteTime() );
    }
}

void Database::AddLayer( std::string layer, GeometryType geomType,
        const std::vector< boost::shared_ptr< TerrainObject > >& features )
{
    tables_.insert( layer, new GeoTable( db_.get(), layer, geomType, features ) );
}

const T_GeoTables& Database::GetTables() const
{
    return tables_;
}
