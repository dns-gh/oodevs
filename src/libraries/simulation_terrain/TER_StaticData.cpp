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
#include <tools/Path.h>
#include <spatialcontainer/StaticTerrainGraph.h>

// -----------------------------------------------------------------------------
// Name: TER_StaticData constructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_StaticData::TER_StaticData( const tools::Path& strGraphArchive, const tools::Path& strNodeArchive, const tools::Path& strLinkArchive, float precision )
{
    pGraph_ = new StaticTerrainGraph( strGraphArchive.ToLocal(), strNodeArchive.ToLocal(), strLinkArchive.ToLocal(), precision );
}

// -----------------------------------------------------------------------------
// Name: TER_StaticData destructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_StaticData::~TER_StaticData()
{
    delete pGraph_;
}
