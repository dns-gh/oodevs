// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanExportManager.h"
#include "UrbanFileExporter.h"
#include "terrain/PointProjector_ABC.h"
#include "gdal_ogr/GdalOgrWorkspace.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: UrbanExportManager constructor
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
UrbanExportManager::UrbanExportManager( const std::string& directory, PointProjector_ABC& projector, const UrbanModel& model )
    : directory_( directory )
    , name_     ( "urbanblock" )
    , projector_( projector )
    , model_    ( model )
{
    gdal_ogr::GdalOgrWorkspace workSpace;
}

// -----------------------------------------------------------------------------
// Name: UrbanExportManager destructor
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
UrbanExportManager::~UrbanExportManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanExportManager::Run
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanExportManager::Run() const
{
    bfs::path exportPath( directory_ );
    if( !bfs::exists( exportPath ) )
        throw std::exception( "export directory doesn't exist" );

    RemoveOldFile( name_ );
    UrbanFileExporter( exportPath.string(), name_, projector_, model_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanExportManager::RemoveOldFile
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanExportManager::RemoveOldFile( const std::string& filename ) const
{
    bfs::path exportPath( directory_ );
    for( bfs::directory_iterator it( exportPath ); it != bfs::directory_iterator(); ++it )
    {
      if( bfs::is_directory( *it ) )
          continue;
      std::string basename = bfs::basename( filename );
      if( bfs::basename( *it ) == filename )
          bfs::remove( *it );
  }
}
