// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Flying/PHY_ActionFly.cpp $
// $Author: Nld $
// $Modtime: 7/03/05 10:24 $
// $Revision: 3 $
// $Workfile: PHY_ActionFly.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ActionFly.h"

#include "PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionFly constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionFly::PHY_ActionFly( MIL_AgentPion& pion )
    : PHY_Action_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_InterfaceFlying >() )
{    
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionFly destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionFly::~PHY_ActionFly()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionFly::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionFly::Execute()
{   
    role_.Fly();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionFly::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionFly::ExecuteSuspended()
{
    assert( false );
    Execute();
}
