// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_Logger.h"
#include "MT_Tools/MT_Logger.h"
#include "pathfind/PathfindLogger.h"
#include "spatialcontainer/Logging.h"

void InitializeTerrainLogger()
{
    pathfind::SetLogger( CreateMTLogger( "pathfind" ));
    spatialcontainer::SetLogger( CreateMTLogger( "spatialcontainer" ));
}

