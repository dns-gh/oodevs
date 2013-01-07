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
#include "LogTable.h"
#include "Database.h"
#include "CreateBlockAutoProcess.h"
#include "CreateBlockProcess.h"
#include "SpatialRequestStatus.h"
#include "terrain/PlanarCartesianProjector.h"
#include "terrain/TerrainFileReader.h"
#include "terrain/Translator.h"

using namespace geostore;

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: GeoStoreManager constructor
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
GeoStoreManager::GeoStoreManager( const bfs::path& path, const SpatialIndexer& index )
    : index_( index )
{
    Initialize( path );
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::Initialize
// Created: AME 2010-07-27
// -----------------------------------------------------------------------------
void GeoStoreManager::Initialize( const bfs::path& path )
{
    path_ = path;
    try
    {
        InitProjector( path_ / "terrain.xml" );
    }
    catch( ... )
    {
        InitProjectorOld( path_ / "World.xml" );
    }
    spatialDb_.reset( new Database( path_ ) );
    spatialDb_->LoadLayers( *trans_ );
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager destructor
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
GeoStoreManager::~GeoStoreManager()
{
    // NOTHING
}

const Database& GeoStoreManager::GetDatabase() const
{
    return *spatialDb_;
}

void GeoStoreManager::CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, double roadWidth, std::vector< geometry::Polygon2f >& urbanBlocks )
{
    try
    {
        CreateBlockAutoProcess process( *spatialDb_, index_, *trans_, roadWidth );
        process.Run( footprint, urbanBlocks );
    }
    catch( ... ) // Created: AME 2010-08-02 Improve exception catching
    {
        // ?????
    }
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::InitProjector
// Created: AME 2010-07-22
// -----------------------------------------------------------------------------
void GeoStoreManager::InitProjectorOld( const bfs::path& worldfile ) 
{
    xml::xifstream xis( worldfile.string() );
    float latitude, longitude, width, height;
    xis >> xml::start( "World" )
        >> xml::content( "Latitude", latitude )
        >> xml::content( "Longitude", longitude )
        >> xml::content( "Width", width )
        >> xml::content( "Height", height );
    proj_.reset( new PlanarCartesianProjector( latitude, longitude ) );
    trans_.reset( new Translator( *proj_, geometry::Vector2d( 0.5 * width, 0.5 * height ) ) );
}

bool GeoStoreManager::CanCreateUrbanBlock( const geometry::Polygon2f& footprint )
{
    CreateBlockProcess process;
    return process.CanCreateBlock( index_, footprint, *trans_ );
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::InitProjector
// Created: AME 2010-07-22
// -----------------------------------------------------------------------------
void GeoStoreManager::InitProjector( const bfs::path& terrainFile ) 
{
    xml::xifstream archive( terrainFile.string() );
    float latitude, longitude, width, height;
    archive >> xml::start( "terrain" )
                >> xml::start( "data" )
                    >> xml::start( "location" )
                        >> xml::start( "center" )
                            >> xml::attribute( "latitude", latitude )
                            >> xml::attribute( "longitude", longitude )
                        >> xml::end
                        >> xml::start( "dimension" )
                            >> xml::attribute( "width", width )
                            >> xml::attribute( "height", height )
                        >> xml::end
                    >> xml::end
                >> xml::end
            >> xml::end;
    proj_.reset( new PlanarCartesianProjector( latitude, longitude ) );
    trans_.reset( new Translator( *proj_, geometry::Vector2d( 0.5 * width, 0.5 * height ) ) );
}
