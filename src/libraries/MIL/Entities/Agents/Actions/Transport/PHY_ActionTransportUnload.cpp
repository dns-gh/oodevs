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

#include "MIL_Pch.h"

#include "PHY_ActionTransportUnload.h"

#include "PHY_RoleAction_Transport.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportUnload constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionTransportUnload::PHY_ActionTransportUnload( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC( pion, diaCall )
    , role_         ( pion.GetRole< PHY_RoleAction_Transport >() )
    , diaReturnCode_( diaCall.GetParameter( 0 ) )
{    
    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportUnload destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionTransportUnload::~PHY_ActionTransportUnload()
{
    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportUnload::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionTransportUnload::Execute()
{   
    int nResult = role_.Unload();
    diaReturnCode_.SetValue( nResult );   
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTransportUnload::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionTransportUnload::ExecuteSuspended()
{
    role_.UnloadSuspended();
}
