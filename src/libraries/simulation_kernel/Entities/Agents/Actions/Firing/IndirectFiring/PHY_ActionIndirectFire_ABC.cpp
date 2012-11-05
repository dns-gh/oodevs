// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_ABC.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 15:01 $
// $Revision: 3 $
// $Workfile: PHY_ActionIndirectFire_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ActionIndirectFire_ABC.h"
#include "PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Decision_ABC.h"

using namespace firing;
// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_ABC constructor
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_ABC::PHY_ActionIndirectFire_ABC( MIL_Agent_ABC& pion, const PHY_DotationCategory* pDotationCategory, float rNbInterventionType )
    : PHY_DecisionCallbackAction_ABC      ( pion )
    , role_               ( pion.GetRole< PHY_RoleAction_IndirectFiring >() )
    , pDotationCategory_  ( 0 )
    , rNbInterventionType_( std::max( 1.f, rNbInterventionType ) )
{
    pDotationCategory_ = pDotationCategory;

    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_ABC destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_ABC::~PHY_ActionIndirectFire_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_ABC::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionIndirectFire_ABC::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}
