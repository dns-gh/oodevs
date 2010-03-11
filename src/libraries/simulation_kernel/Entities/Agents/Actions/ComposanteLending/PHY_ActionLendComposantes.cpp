// *****************************************************************************
//
// $Created: JVT 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/ComposanteLending/PHY_ActionLendComposantes.cpp $
// $Author: Jvt $
// $Modtime: 12/05/05 15:25 $
// $Revision: 1 $
// $Workfile: PHY_ActionLendComposantes.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionLendComposantes.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposantes constructor
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
PHY_ActionLendComposantes::PHY_ActionLendComposantes( MIL_AgentPion& pion, DEC_RolePion_Decision* pAgent, unsigned int nbrToLend, T_ComposantePredicate predicate )
    : PHY_DecisionCallbackAction_ABC    ( pion )
    , role_             ( pion.GetRole< PHY_RolePion_Composantes >() )
    , pTarget_          ( 0 )
    , predicate_        ( predicate )
    , nNbrToLend_       ( nbrToLend )
    , bLoanDone_        ( false )
{
    assert( pAgent );
    
    pTarget_ = &pAgent->GetPion().GetRole< PHY_RolePion_Composantes >();
   
    nTimer_ = role_.GetLentComposantesTravelTime( *pTarget_, nNbrToLend_, std::mem_fun_ref( predicate_ ) );
    MIL_Report::PostEvent( pion, MIL_Report::eReport_EquipmentLoanInProgress );
    
    Callback( false );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposantes destructor
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
PHY_ActionLendComposantes::~PHY_ActionLendComposantes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposantes::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionLendComposantes::StopAction()
{
    if( !bLoanDone_ )
        MIL_Report::PostEvent( role_.GetPion(), MIL_Report::eReport_EquipmentLoanCanceled );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposantes::Execute
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void PHY_ActionLendComposantes::Execute()
{
    if ( !bLoanDone_ && !nTimer_-- )
    {
        assert( pTarget_ );
        const unsigned int nNbrLent = role_.LendComposantes( *pTarget_, nNbrToLend_, std::mem_fun_ref( predicate_ ) );

        if( nNbrLent == 0 )
            MIL_Report::PostEvent( role_.GetPion(), MIL_Report::eReport_EquipmentLoanImpossible );
        else
        {
            MIL_Report::PostEvent( pTarget_->GetPion(), MIL_Report::eReport_EquipmentLent );
            if( nNbrLent < nNbrToLend_ )
                MIL_Report::PostEvent( role_.GetPion(), MIL_Report::eReport_EquipmentLoanPartiallyDone );
            else
                MIL_Report::PostEvent( role_.GetPion(), MIL_Report::eReport_EquipmentLoanDone );
        }

        bLoanDone_ = true;
        Callback( true );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposantes::ExecuteSuspended
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void PHY_ActionLendComposantes::ExecuteSuspended()
{
    Execute();
}
