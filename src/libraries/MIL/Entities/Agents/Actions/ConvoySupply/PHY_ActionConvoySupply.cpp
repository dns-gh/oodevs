// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoySupply.cpp $
// $Author: Nld $
// $Modtime: 10/02/05 10:59 $
// $Revision: 1 $
// $Workfile: PHY_ActionConvoySupply.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_ActionConvoySupply.h"

#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoySupply constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoySupply::PHY_ActionConvoySupply( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC( pion, diaCall )
    , role_         ( pion.GetRole< PHY_RolePion_Supply >() )
    , diaReturnCode_( diaCall.GetParameter( 0 ) )
{    
    diaReturnCode_.SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoySupply destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionConvoySupply::~PHY_ActionConvoySupply()
{
    diaReturnCode_.SetValue( true );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoySupply::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionConvoySupply::Execute()
{   
    bool bResult = role_.ConvoySupply();
    diaReturnCode_.SetValue( bResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionConvoySupply::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionConvoySupply::ExecuteSuspended()
{
    // NOTHING
}
