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

#include "MIL_pch.h"

#include "PHY_ActionConvoyUnload.h"

#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyUnload constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoyUnload::PHY_ActionConvoyUnload( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC( pion, diaCall )
    , role_         ( pion.GetRole< PHY_RolePion_Supply >() )
    , diaReturnCode_( diaCall.GetParameter( 0 ) )
{    
    diaReturnCode_.SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyUnload destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoyUnload::~PHY_ActionConvoyUnload()
{
    diaReturnCode_.SetValue( true );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyUnload::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionConvoyUnload::Execute()
{   
    bool bResult = role_.ConvoyUnload();
    diaReturnCode_.SetValue( bResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoyUnload::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionConvoyUnload::ExecuteSuspended()
{
    // NOTHING
}
