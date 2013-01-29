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
    UrbanFileExporter( directory_, name_, projector_, model_ );
}
