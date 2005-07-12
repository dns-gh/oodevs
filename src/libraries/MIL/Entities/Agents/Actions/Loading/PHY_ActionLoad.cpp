// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Loading/PHY_ActionLoad.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionLoad.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_ActionLoad.h"

#include "PHY_RoleAction_Loading.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionLoad constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionLoad::PHY_ActionLoad( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC( pion, diaCall )
    , role_          ( pion.GetRole< PHY_RoleAction_Loading >() )
    , diaReturnCode_ ( diaCall.GetParameter( 0 ) )
{    
    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLoad destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionLoad::~PHY_ActionLoad()
{
    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionLoad::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionLoad::Execute()
{   
    int nResult = role_.Load();
    diaReturnCode_.SetValue( nResult );   
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLoad::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionLoad::ExecuteSuspended()
{
    role_.LoadSuspended();
}
