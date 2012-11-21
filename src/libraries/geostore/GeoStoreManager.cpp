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
GeoStoreManager::GeoStoreManager( const std::string& path, UrbanModel& model )
    : urbanModel_( model )
    , spatialDb_( 0 )
    , trans_( 0 )
{
    Initialize( path );
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager constructor
// Created: AME 2010-07-27
// -----------------------------------------------------------------------------
GeoStoreManager::GeoStoreManager( UrbanModel& model )
    : urbanModel_( model )
    , spatialDb_( 0 )
    , trans_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::Initialize
// Created: AME 2010-07-27
// -----------------------------------------------------------------------------
void GeoStoreManager::Initialize( const std::string& path )
{
    path_ = path;
    bfs::path directory( path_ );
    spatialDb_ = new Database( directory.string() );
    try
    {
        InitProjector( ( directory / "terrain.xml" ).string() );
    }
    catch ( ... )
    {
        InitProjectorOld( ( directory / "World.xml" ).string() );
    }
    LoadTerrainFiles();
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager destructor
// Created: AME 2010-07-19
// -----------------------------------------------------------------------------
GeoStoreManager::~GeoStoreManager()
{
    delete spatialDb_;
    delete trans_;
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::LoadTerrainFiles
// Created: AME 2010-07-20
// -----------------------------------------------------------------------------
void GeoStoreManager::LoadTerrainFiles()
{
    bool status = spatialDb_->logTable_->status_;
    bfs::path directory( path_ );
    bfs::path graphicsDirectory( directory / "graphics" );    
    for( bfs::directory_iterator it( graphicsDirectory ); it != bfs::directory_iterator(); ++it )
        if( bfs::extension( *it ) == ".bin" && bfs::basename( *it ) != "preview" )
        {
            if ( status )
                spatialDb_->GetTable( bfs::basename( *it ) );
            else
            {
                TerrainFileReader terrainFile( it->path().string(), *trans_ );
                spatialDb_->CreateTable( terrainFile );
            }
        }
    spatialDb_->logTable_->UpdateStatus( std::string( "yes" ) );
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::CreateUrbanBlocksOnCities
// Created: AME 2010-07-21
// -----------------------------------------------------------------------------
SpatialRequestStatus* GeoStoreManager::CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, kernel::UrbanObject_ABC& parent )
{
    SpatialRequestStatus* processStatus = new SpatialRequestStatus();
    try
    {
        CreateBlockAutoProcess process( *spatialDb_, *processStatus );
        process.Run( footprint, urbanModel_, parent, *trans_ );
        return processStatus;
    }
    catch( ... ) // Created: AME 2010-08-02 Improve exception catching
    {
        return processStatus;
    }
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::InitProjector
// Created: AME 2010-07-22
// -----------------------------------------------------------------------------
void GeoStoreManager::InitProjectorOld( const std::string& worldfile ) 
{
    xml::xifstream xis( worldfile );
    float latitude, longitude, width, height;
    xis >> xml::start( "World" )
        >> xml::content( "Latitude", latitude )
        >> xml::content( "Longitude", longitude )
        >> xml::content( "Width", width )
        >> xml::content( "Height", height );
    trans_ = new Translator( *new PlanarCartesianProjector( latitude, longitude ), geometry::Vector2d( 0.5 * width, 0.5 * height ) );
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::InitProjector
// Created: AME 2010-07-22
// -----------------------------------------------------------------------------
void GeoStoreManager::InitProjector( const std::string& terrainFile ) 
{
    xml::xifstream archive( terrainFile );
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
                >> xml::end()
            >> xml::end();
    trans_ = new Translator( *new PlanarCartesianProjector( latitude, longitude ), geometry::Vector2d( 0.5 * width, 0.5 * height ) );
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::BlockAutoProcess
// Created: AME 2010-08-05
// -----------------------------------------------------------------------------
bool GeoStoreManager::BlockAutoProcess( const geometry::Polygon2f& footprint )
{
    CreateBlockProcess process;
    return process.CanCreateBlock( urbanModel_, footprint, *trans_ );
}

// -----------------------------------------------------------------------------
// Name: GeoStoreManager::IsInitialized
// Created: CMA 2011-10-19
// -----------------------------------------------------------------------------
bool GeoStoreManager::IsInitialized() const
{
    return spatialDb_ != 0;
}
