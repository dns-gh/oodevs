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
#include <terrain/TerrainFileReader.h>
#pragma warning ( push, 0 )
#include <boost/locale.hpp>
#pragma warning ( pop )

using namespace geostore;

namespace blc = boost::locale::conv;

namespace
{
    sqlite3* Open( const tools::Path& path )
    {
        sqlite3* db = 0;
        if( SQLITE_OK !=
            sqlite3_open_v2(
                blc::utf_to_utf< char >( path.ToBoost().wstring() ).c_str(),
                &db,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                0 ) )
            throw MASA_EXCEPTION_SQLITE( "Failed to initialize database" );
        sqlite3_enable_load_extension( db, 1 );
        return db;
    }
}

Database::Database( const tools::Path& dbFile, const tools::Path& layersDir, PointProjector_ABC& projector )
    : db_  ( Open( dbFile ), &sqlite3_close )
    , log_ ( new LogTable( db_.get() ) )
{
    spatialite_init( 0 );
    if( sqlite3_exec( db_.get(), "SELECT load_extension('spatialite.dll')", 0, 0, 0 ) != SQLITE_OK )
        throw std::runtime_error( "cannot find spatialite DLL to load as sqlite extension" );
    ProjectionTable projection( db_.get() ); // Just used to initialize the projection table
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
    {
        tables_.insert( layer, new GeoTable( db_.get(), layer ) );
    }
    else
    {
        const TerrainFileReader reader( file.ToLocal(), projector );
        AddLayer( layer, static_cast< GeometryType >( reader.GetGeomType() ), reader.GetFeatures() );
        log_->SetLastAccessTime( layer, file.LastWriteTime() );
    }
}

void Database::AddLayer( std::string layer, GeometryType geomType, const std::vector< TerrainObject* >& features )
{
    tables_.insert( layer, new GeoTable( db_.get(), layer, geomType, features ) );
}

const T_GeoTables& Database::GetTables() const
{
    return tables_;
}
