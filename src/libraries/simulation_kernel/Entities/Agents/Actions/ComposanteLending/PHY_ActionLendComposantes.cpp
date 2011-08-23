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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposantes constructor
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
PHY_ActionLendComposantes::PHY_ActionLendComposantes( MIL_AgentPion& pion, DEC_Decision_ABC* pStart, DEC_Decision_ABC* pTarget, unsigned int nbrToLend, T_ComposantePredicate predicate )
    : PHY_DecisionCallbackAction_ABC    ( pion )
    , pion_             ( pion )
    , target_           ( pTarget->GetPion() )
    , predicate_        ( predicate )
    , nNbrToLend_       ( nbrToLend )
    , bLoanDone_        ( false )
{
    PHY_RolePion_Composantes& roleDepart = pStart->GetPion().GetRole< PHY_RolePion_Composantes >();
    nTimer_ = roleDepart.GetLentComposantesTravelTime( target_, nNbrToLend_, std::mem_fun_ref( predicate_ ) );

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
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_EquipmentLoanCanceled );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposantes::Execute
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void PHY_ActionLendComposantes::Execute()
{
    if( !bLoanDone_ && !nTimer_-- )
    {
        const unsigned int nNbrLent = pion_.GetRole< PHY_RolePion_Composantes >().LendComposantes( target_, nNbrToLend_, std::mem_fun_ref( predicate_ ) );

        if( nNbrLent == 0 )
            MIL_Report::PostEvent( pion_, MIL_Report::eReport_EquipmentLoanImpossible );
        else
        {
            MIL_Report::PostEvent( pion_, MIL_Report::eReport_EquipmentLent );
            if( nNbrLent < nNbrToLend_ )
                MIL_Report::PostEvent( pion_, MIL_Report::eReport_EquipmentLoanPartiallyDone );
            else
                MIL_Report::PostEvent( pion_, MIL_Report::eReport_EquipmentLoanDone );
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
