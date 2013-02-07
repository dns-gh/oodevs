// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "GeoStoreManager.h"
#include "Database.h"
#include "CreateBlockAutoProcess.h"
#include "CreateBlockProcess.h"
#include <terrain/TerrainFileReader.h>
#include <terrain/PointProjector.h>

using namespace geostore;
namespace bfs = boost::filesystem;

GeoStoreManager::GeoStoreManager( const bfs::path& path, const SpatialIndexer& index )
    : index_    ( index )
    , projector_( new PointProjector( path ) )
{
    bfs::path layersDir = path / "Graphics";
    bfs::path dbFile = layersDir / "geostore.sqlite";
    try
    {
        database_.reset( new Database( dbFile, layersDir, *projector_ ) );
    }
    catch( ... )
    {
        bfs::remove( dbFile );
        database_.reset( new Database( dbFile, layersDir, *projector_ ) );
    }
}

GeoStoreManager::~GeoStoreManager()
{
    // NOTHING
}

const Database& GeoStoreManager::GetDatabase() const
{
    return *database_;
}

void GeoStoreManager::CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, double roadWidth, std::vector< geometry::Polygon2f >& urbanBlocks )
{
    try
    {
        CreateBlockAutoProcess process( database_->GetTables(), index_,
                *projector_, roadWidth );
        process.Run( footprint, urbanBlocks );
    }
    catch( ... ) // Created: AME 2010-08-02 Improve exception catching
    {
        // ?????
    }
}

std::vector< const kernel::UrbanObject_ABC* > GeoStoreManager::IntersectedBlocks( const geometry::Polygon2f& footprint )
{
    CreateBlockProcess process;
    return process.IntersectedBlocks( index_, footprint, *projector_ );
}
