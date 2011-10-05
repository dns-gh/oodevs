// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePion_ABC.cpp $
// $Author: Nld $
// $Modtime: 28/10/04 9:48 $
// $Revision: 3 $
// $Workfile: PHY_ActionDirectFirePion_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ActionDirectFirePion_ABC.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Decision/DEC_Tools.h"

using namespace firing;
// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePion_ABC constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePion_ABC::PHY_ActionDirectFirePion_ABC( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, double percentage, int firingMode, int ammoDotationClass, firing::PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType, firing::PHY_DirectFireData::E_ComposanteFiredType nComposanteFiredType )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_                       ( pion.GetRole< PHY_RoleAction_DirectFiring >() )
    , pEnemy_                     ( pEnemy )
    , rPercentageComposantesToUse_( std::max( 0., std::min( 1., percentage ) ) )
    , nFiringMode_                ( (firing::PHY_DirectFireData::E_FiringMode)firingMode )
    , nComposanteFiringType_      ( nComposanteFiringType )
    , nComposanteFiredType_       ( nComposanteFiredType  )
    , pAmmoDotationClass_         ( 0 )
    , pFireResult_                ( 0 )
    , mustReport_                 ( true )
{
    if( ammoDotationClass != -1 ) // $$$$ LDC FIXME Varargs hidden here...
       pAmmoDotationClass_ = PHY_AmmoDotationClass::Find( ammoDotationClass );

    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePion_ABC destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePion_ABC::~PHY_ActionDirectFirePion_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePion_ABC::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionDirectFirePion_ABC::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
    if( pFireResult_ )
        pFireResult_->DecRef();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePion_ABC::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionDirectFirePion_ABC::Execute()
{
    bool bMustRefResult = ( pFireResult_ == 0 );
    int nResult = role_.FirePion( pEnemy_, nFiringMode_, rPercentageComposantesToUse_, nComposanteFiringType_, nComposanteFiredType_, pFireResult_, mustReport_, pAmmoDotationClass_ );
    Callback( nResult );

    if( pFireResult_ && bMustRefResult )
        pFireResult_->IncRef();
    mustReport_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePion_ABC::ExecuteSuspended
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_ActionDirectFirePion_ABC::ExecuteSuspended()
{
    role_.FirePionSuspended( pEnemy_, !mustReport_ );
    mustReport_ = true;
}
