// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-28 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_World.cpp $
// $Author: Nld $
// $Modtime: 11/04/05 15:27 $
// $Revision: 5 $
// $Workfile: TER_World.cpp $
//
// *****************************************************************************

#include "simulation_terrain_pch.h"

#include "TER_PopulationManager.h"

#include "MT_Tools/MT_Rect.h"

// -----------------------------------------------------------------------------
// Name: TER_PopulationManager constructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationManager::TER_PopulationManager( const MT_Rect& extent )
    : concentrationManager_ ( extent )
    , flowManager_          ( extent )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TER_PopulationManager destructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationManager::~TER_PopulationManager()
{
    // NOTHING
}
