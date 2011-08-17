// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_StaticData.h"
#include <spatialcontainer/StaticTerrainGraph.h>

// -----------------------------------------------------------------------------
// Name: TER_StaticData constructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_StaticData::TER_StaticData( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive, float precision )
{
    pGraph_ = new StaticTerrainGraph( strGraphArchive, strNodeArchive, strLinkArchive, precision );
}

// -----------------------------------------------------------------------------
// Name: TER_StaticData destructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_StaticData::~TER_StaticData()
{
    delete pGraph_;
}
