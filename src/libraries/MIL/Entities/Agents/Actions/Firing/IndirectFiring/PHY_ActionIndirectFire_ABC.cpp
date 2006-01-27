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

#include "MIL_pch.h"

#include "PHY_ActionIndirectFire_ABC.h"

#include "PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_ABC constructor
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_ABC::PHY_ActionIndirectFire_ABC( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC       ( pion, diaCall )
    , role_                ( pion.GetRole< PHY_RoleAction_IndirectFiring >() )
    , diaReturnCode_       ( diaCall.GetParameter( 0 ) )
    , pIndirectWeaponClass_( PHY_IndirectFireDotationClass::Find( diaCall.GetParameter( 1 ).ToId() ) )
    , rNbInterventionType_ ( std::max( (float)1., diaCall.GetParameter( 2 ).ToFloat() ) )
{
    assert( pIndirectWeaponClass_ );
    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_ABC destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_ABC::~PHY_ActionIndirectFire_ABC()
{
    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
}

