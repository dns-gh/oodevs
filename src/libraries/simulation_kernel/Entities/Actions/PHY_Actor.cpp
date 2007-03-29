// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/PHY_Actor.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 18:40 $
// $Revision: 42 $
// $Workfile: PHY_Actor.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_Actor.h"

#include "PHY_Action_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_Actor constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
PHY_Actor::PHY_Actor()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
PHY_Actor::~PHY_Actor()
{
    while( !actions_.empty() )
        delete *actions_.begin();
}

// =============================================================================
// UPDATES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Actor::UpdateActions
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_Actor::UpdateActions()
{
    for( CIT_ActionSet itAction = actions_.begin(); itAction != actions_.end(); ++itAction )
        (**itAction).Update();
}
