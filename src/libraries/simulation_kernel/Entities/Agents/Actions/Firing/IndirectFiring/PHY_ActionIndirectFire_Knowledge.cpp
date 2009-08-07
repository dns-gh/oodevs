// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Knowledge.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 15:01 $
// $Revision: 4 $
// $Workfile: PHY_ActionIndirectFire_Knowledge.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionIndirectFire_Knowledge.h"
#include "PHY_RoleAction_IndirectFiring.h"
#include "MIL_AgentServer.h"
#include "Decision/DEC_Tools.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Knowledge constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_Knowledge::PHY_ActionIndirectFire_Knowledge( MIL_AgentPion& pion, const PHY_DotationCategory* pDotationCategory, float rNbInterventionType, unsigned int nTargetKnowledgeID )
    : PHY_ActionIndirectFire_ABC( pion, pDotationCategory, rNbInterventionType )
    , pEffect_                  ( 0 )
{
    if( pDotationCategory_ && pDotationCategory_->CanBeUsedForIndirectFire() )
    {
        pEffect_ = new MIL_Effect_IndirectFire( pion, nTargetKnowledgeID, *pDotationCategory_->GetIndirectFireData(), rNbInterventionType_ );
        pEffect_->IncRef();
        MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( *pEffect_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Knowledge destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_Knowledge::~PHY_ActionIndirectFire_Knowledge()
{
    if( pEffect_ )
    {
        pEffect_->ForceFlying();
        pEffect_->DecRef();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Knowledge::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionIndirectFire_Knowledge::Execute()
{
    const int nResult = role_.Fire( pEffect_ );
    Callback( nResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Knowledge::ExecuteSuspended
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_ActionIndirectFire_Knowledge::ExecuteSuspended()
{
    role_.FireSuspended();
}
