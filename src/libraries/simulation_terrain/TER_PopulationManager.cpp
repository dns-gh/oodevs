// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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

// -----------------------------------------------------------------------------
// Name: TER_PopulationManager::GetConcentrationManager
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationConcentrationManager& TER_PopulationManager::GetConcentrationManager()
{
    return concentrationManager_;
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationManager::GetFlowManager
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlowManager& TER_PopulationManager::GetFlowManager()
{
    return flowManager_;
}
