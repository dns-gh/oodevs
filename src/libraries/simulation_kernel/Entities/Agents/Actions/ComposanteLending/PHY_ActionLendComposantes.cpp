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
#include "Entities/RC/MIL_RC.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposantes constructor
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
PHY_ActionLendComposantes::PHY_ActionLendComposantes( MIL_AgentPion& pion, DIA_Call_ABC& call, T_ComposantePredicate predicate )
    : PHY_Action_ABC    ( pion, call )
    , role_             ( pion.GetRole< PHY_RolePion_Composantes >() )
    , nNbrToLend_       ( (uint)call.GetParameter( 1 ).ToFloat() )
    , bLoanDone_        ( false )
    , pTarget_          ( 0 )
    , diaReturnVariable_( call.GetParameter( 2 ) )
    , predicate_        ( predicate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pAgent = call.GetParameter( 0 ).ToUserObject( pAgent );
    assert( pAgent );
    
    pTarget_ = &pAgent->GetPion().GetRole< PHY_RolePion_Composantes >();
   
    nTimer_ = role_.GetLentComposantesTravelTime( *pTarget_, nNbrToLend_, std::mem_fun_ref( predicate_ ) );
    MIL_RC::pRcPretMaterielEnCours_->Send( pion, MIL_RC::eRcTypeOperational );
    
    diaReturnVariable_.SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposantes destructor
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
PHY_ActionLendComposantes::~PHY_ActionLendComposantes()
{
    if( diaReturnVariable_.ToBool() == false )
        MIL_RC::pRcPretMaterielAnnule_->Send( role_.GetPion(), MIL_RC::eRcTypeOperational );
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
        const uint nNbrLent = role_.LendComposantes( *pTarget_, nNbrToLend_, std::mem_fun_ref( predicate_ ) );

        if( nNbrLent == 0 )
            MIL_RC::pRcPretMaterielImpossible_->Send( role_.GetPion(), MIL_RC::eRcTypeOperational );
        else
        {
            MIL_RC::pRcMaterielPrete_->Send( pTarget_->GetPion(), MIL_RC::eRcTypeOperational );
            if( nNbrLent < nNbrToLend_ )
                MIL_RC::pRcPretMaterielPartiellementEffectue_->Send( role_.GetPion(), MIL_RC::eRcTypeOperational );
            else
                MIL_RC::pRcPretMaterielEffectue_->Send( role_.GetPion(), MIL_RC::eRcTypeOperational );
        }

        bLoanDone_ = true;
        diaReturnVariable_.SetValue( true );
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
