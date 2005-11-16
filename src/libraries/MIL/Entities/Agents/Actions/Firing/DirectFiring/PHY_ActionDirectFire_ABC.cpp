// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFire_ABC.cpp $
// $Author: Nld $
// $Modtime: 28/10/04 9:48 $
// $Revision: 3 $
// $Workfile: PHY_ActionDirectFire_ABC.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ActionDirectFire_ABC.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFire_ABC constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFire_ABC::PHY_ActionDirectFire_ABC( MIL_AgentPion& pion, DIA_Call_ABC& diaCall, PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType )
    : PHY_Action_ABC              ( pion )
    , role_                       ( pion.GetRole< PHY_RoleAction_DirectFiring >() )
    , diaReturnCode_              (       diaCall.GetParameter( 0 )         )
    , nTargetKnowledgeID_         ( (uint)diaCall.GetParameter( 1 ).ToPtr  () )
    , rPercentageComposantesToUse_(  std::max( 0., std::min( 1., (MT_Float)diaCall.GetParameter( 2 ).ToFloat() ) ) )
    , nFiringMode_                ( (PHY_DirectFireData::E_FiringMode)diaCall.GetParameter( 3 ).ToId   () )
    , nComposanteFiringType_      ( nComposanteFiringType )
    , pAmmoDotationClass_         ( 0 )
    , pFireResult_                ( 0 )  
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaCall.GetParameter( 1 ) ) );

    if( diaCall.GetParameters().GetParameters().size() > 4 )
        pAmmoDotationClass_ = PHY_AmmoDotationClass::Find( diaCall.GetParameter( 4 ).ToId() );

    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFire_ABC destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFire_ABC::~PHY_ActionDirectFire_ABC()
{
    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
    if( pFireResult_ )
        pFireResult_->DecRef();
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFire_ABC::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionDirectFire_ABC::Execute()
{
    bool bMustRefResult = ( pFireResult_ == 0 );
    int nResult = role_.Fire( nTargetKnowledgeID_, nFiringMode_, rPercentageComposantesToUse_, nComposanteFiringType_, pFireResult_, pAmmoDotationClass_ );
    diaReturnCode_.SetValue( nResult );

    if( pFireResult_ && bMustRefResult )
        pFireResult_->IncRef();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFire_ABC::ExecuteSuspended
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_ActionDirectFire_ABC::ExecuteSuspended()
{
    role_.FireSuspended( nTargetKnowledgeID_ );
}
