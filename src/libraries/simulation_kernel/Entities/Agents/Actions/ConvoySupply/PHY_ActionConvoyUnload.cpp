// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoyUnload.cpp $
// $Author: Nld $
// $Modtime: 10/02/05 10:59 $
// $Revision: 1 $
// $Workfile: PHY_ActionConvoyUnload.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionConvoyUnload.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyUnload constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoyUnload::PHY_ActionConvoyUnload( MIL_AgentPion& pion )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RolePion_Supply >() )
{    
    Callback( false );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyUnload destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoyUnload::~PHY_ActionConvoyUnload()
{
    Callback( true );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyUnload::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionConvoyUnload::Execute()
{   
    bool bResult = role_.ConvoyUnload();
    Callback( bResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyUnload::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionConvoyUnload::ExecuteSuspended()
{
    // NOTHING
}
