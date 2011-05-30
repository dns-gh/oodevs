// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ZOPerceptionComputer.h"
#include "PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"

// -----------------------------------------------------------------------------
// Name: PHY_ZOPerceptionComputer constructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
PHY_ZOPerceptionComputer::PHY_ZOPerceptionComputer( const MIL_Agent_ABC& perceiver )
    : PHY_PerceptionComputer_ABC( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ZOPerceptionComputer destructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
PHY_ZOPerceptionComputer::~PHY_ZOPerceptionComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ZOPerceptionComputer::ComputePerception
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_ZOPerceptionComputer::ComputePerception( const MIL_Agent_ABC& target ) const
{
    const PHY_RoleInterface_Perceiver& rolePerceiver = perceiver_.GetRole< PHY_RoleInterface_Perceiver >();
    const PHY_RoleInterface_Perceiver::T_SurfaceAgentMap& surfaces = rolePerceiver.GetSurfacesAgent();
    const PHY_PerceptionLevel* pBestLevel = &PHY_PerceptionLevel::notSeen_;
    for( PHY_RoleInterface_Perceiver::CIT_SurfaceAgentMap itSurface = surfaces.begin(); itSurface != surfaces.end(); ++itSurface )
    {
        const PHY_PerceptionLevel& currentLevel = itSurface->second.ComputePerception( rolePerceiver, target );
        if( currentLevel > *pBestLevel )
        {
            pBestLevel = &currentLevel;
            if( pBestLevel->IsBestLevel() )
                return *pBestLevel;
        }
    }
    return *pBestLevel;
}
