// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Transport/PHY_ActionTransportLoad.cpp $
// $Author: Nld $
// $Modtime: 19/04/05 16:42 $
// $Revision: 2 $
// $Workfile: PHY_ActionTransportLoad.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionTransportLoad.h"
#include "PHY_RoleAction_Transport.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportLoad constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionTransportLoad::PHY_ActionTransportLoad( MIL_AgentPion& pion )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< transport::PHY_RoleAction_Transport >() )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportLoad destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionTransportLoad::~PHY_ActionTransportLoad()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportLoad::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionTransportLoad::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportLoad::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionTransportLoad::Execute()
{
    int nResult = role_.Load();
    Callback( nResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportLoad::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionTransportLoad::ExecuteSuspended()
{
    role_.LoadSuspended();
}
