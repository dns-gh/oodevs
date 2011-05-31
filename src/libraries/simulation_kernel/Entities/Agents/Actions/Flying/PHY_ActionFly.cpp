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
#include "Entities/MIL_Entity_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionFly constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionFly::PHY_ActionFly( MIL_Entity_ABC& entity )
    : PHY_Action_ABC()
    , role_         ( entity.GetRole< PHY_RoleAction_InterfaceFlying >() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionFly destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionFly::~PHY_ActionFly()
{
    // NOTHING
}

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

// -----------------------------------------------------------------------------
// Name: PHY_ActionFly::Stop
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionFly::Stop()
{
    // NOTHING
}
