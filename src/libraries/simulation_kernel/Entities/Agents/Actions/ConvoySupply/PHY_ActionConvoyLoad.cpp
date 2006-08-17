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

#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyLoad constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoyLoad::PHY_ActionConvoyLoad( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC( pion, diaCall )
    , role_         ( pion.GetRole< PHY_RolePion_Supply >() )
    , diaReturnCode_( diaCall.GetParameter( 0 ) )
{    
    diaReturnCode_.SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyLoad destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoyLoad::~PHY_ActionConvoyLoad()
{
    diaReturnCode_.SetValue( true );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyLoad::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionConvoyLoad::Execute()
{   
    bool bResult = role_.ConvoyLoad();
    diaReturnCode_.SetValue( bResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyLoad::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionConvoyLoad::ExecuteSuspended()
{
    // NOTHING
}
