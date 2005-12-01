// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePopulation.cpp $
// $Author: Nld $
// $Modtime: 28/10/04 9:48 $
// $Revision: 3 $
// $Workfile: PHY_ActionDirectFirePopulation.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ActionDirectFirePopulation.h"

#include "PHY_RoleAction_DirectFiring.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePopulation constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePopulation::PHY_ActionDirectFirePopulation( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC              ( pion )
    , role_                       ( pion.GetRole< PHY_RoleAction_DirectFiring >() )
    , diaReturnCode_              (       diaCall.GetParameter( 0 )         )
    , nTargetKnowledgeID_         ( (uint)diaCall.GetParameter( 1 ).ToPtr  () )
    , pFireResult_                ( 0 )  
{
    assert( DEC_Tools::CheckTypeConnaissancePopulation( diaCall.GetParameter( 1 ) ) );
    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePopulation destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePopulation::~PHY_ActionDirectFirePopulation()
{
//    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
    if( pFireResult_ )
        pFireResult_->DecRef();
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePopulation::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionDirectFirePopulation::Execute()
{
    bool bMustRefResult = ( pFireResult_ == 0 );
    int nResult = role_.FirePopulation( nTargetKnowledgeID_, pFireResult_ );
    diaReturnCode_.SetValue( nResult );

    if( pFireResult_ && bMustRefResult )
        pFireResult_->IncRef();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePopulation::ExecuteSuspended
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_ActionDirectFirePopulation::ExecuteSuspended()
{
    role_.FirePopulationSuspended( nTargetKnowledgeID_ );
}

