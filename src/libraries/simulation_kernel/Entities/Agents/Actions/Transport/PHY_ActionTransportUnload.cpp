// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Transport/PHY_ActionTransportUnload.cpp $
// $Author: Nld $
// $Modtime: 19/04/05 16:42 $
// $Revision: 2 $
// $Workfile: PHY_ActionTransportUnload.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionTransportUnload.h"
#include "PHY_RoleAction_Transport.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportUnload constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionTransportUnload::PHY_ActionTransportUnload( MIL_AgentPion& pion, MT_Vector2D* position )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_    ( pion.GetRole< transport::PHY_RoleAction_Transport >() )
{
    Callback( role_.GetInitialReturnCode() );
    if( position )
        position_.reset( new MT_Vector2D( *position ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportUnload destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionTransportUnload::~PHY_ActionTransportUnload()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportUnload::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionTransportUnload::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportUnload::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionTransportUnload::Execute()
{
    int nResult = role_.Unload( position_.get() );
    Callback( nResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportUnload::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionTransportUnload::ExecuteSuspended()
{
    role_.UnloadSuspended();
}
