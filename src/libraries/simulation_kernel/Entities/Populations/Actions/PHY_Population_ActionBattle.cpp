//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Population_ActionBattle.h"
#include "Entities/MIL_EntityManager.h"
#include "simulation_kernel/MIL_AgentServer.h"
#include "simulation_kernel/Entities/Populations/MIL_Population.h"

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionBattle constructor
// Created: LGY 2012-02-14
// -----------------------------------------------------------------------------
PHY_Population_ActionBattle::PHY_Population_ActionBattle( MIL_Population& attacker, unsigned int nID )
    : attacker_( attacker )
    , defender_( 0 )
{
    defender_ = MIL_AgentServer::GetWorkspace().GetEntityManager().FindPopulation( nID );
}
// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionBattle destructor
// Created: LGY 2012-02-14
// -----------------------------------------------------------------------------
PHY_Population_ActionBattle::~PHY_Population_ActionBattle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionBattle::Execute
// Created: LGY 2012-02-14
// -----------------------------------------------------------------------------
void PHY_Population_ActionBattle::Execute()
{
    if( defender_ )
        attacker_.Attack( *defender_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionBattle::ExecuteSuspended
// Created: LGY 2012-02-14
// -----------------------------------------------------------------------------
void PHY_Population_ActionBattle::ExecuteSuspended()
{
    Execute();
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionBattle::Stop
// Created: LGY 2012-02-14
// -----------------------------------------------------------------------------
void PHY_Population_ActionBattle::Stop()
{
    // NOTHING
}
