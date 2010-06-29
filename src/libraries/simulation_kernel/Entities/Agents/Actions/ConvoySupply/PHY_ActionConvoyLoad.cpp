// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoyLoad.cpp $
// $Author: Nld $
// $Modtime: 10/02/05 10:59 $
// $Revision: 1 $
// $Workfile: PHY_ActionConvoyLoad.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionConvoyLoad.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyLoad constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoyLoad::PHY_ActionConvoyLoad( MIL_Agent_ABC& pion )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleInterface_Supply >() )
{
    Callback( false );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyLoad destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoyLoad::~PHY_ActionConvoyLoad()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyLoad::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionConvoyLoad::StopAction()
{
    Callback( true );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyLoad::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionConvoyLoad::Execute()
{
    bool bResult = role_.ConvoyLoad();
    Callback( bResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyLoad::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionConvoyLoad::ExecuteSuspended()
{
    // NOTHING
}
