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
#include <terrain/PointProjector.h>
#include <boost/bind.hpp>

using namespace geostore;

namespace
{
    Database* Load( const tools::Path& layers, tools::Path& file, PointProjector_ABC& projector )
    {
        try
        {
            return new Database( file, layers, projector );
        }
        catch( ... )
        {
            file = tools::Path::TemporaryFile();
            return new Database( file, layers, projector );
        }
    }
}

GeoStoreManager::GeoStoreManager( const tools::Path& terrain, const SpatialIndexer& index )
    : index_    ( index )
    , file_     ( terrain / "Graphics/geostore.sqlite" )
    , temporary_( file_ )
    , projector_( new PointProjector( terrain.ToBoost() ) )
    , database_ ( Load( terrain / "Graphics", temporary_, *projector_ ) )
{
    // NOTHING
}

GeoStoreManager::~GeoStoreManager()
{
    try
    {
        temporary_.Rename( file_ );
    }
    catch( ... )
    {}
}

const Database& GeoStoreManager::GetDatabase() const
{
    return *database_;
}

void GeoStoreManager::CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, double roadWidth, std::vector< geometry::Polygon2f >& urbanBlocks )
{
    try
    {
        CreateBlockAutoProcess process( database_->GetTables(), index_, *projector_, roadWidth );
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
