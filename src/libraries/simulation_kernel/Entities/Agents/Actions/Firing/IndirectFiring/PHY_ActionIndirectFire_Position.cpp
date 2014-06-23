// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 15:01 $
// $Revision: 4 $
// $Workfile: PHY_ActionIndirectFire_Position.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionIndirectFire_Position.h"
#include "PHY_RoleAction_IndirectFiring.h"
#include "MIL_AgentServer.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_Position::PHY_ActionIndirectFire_Position( MIL_Agent_ABC& pion, const PHY_DotationCategory* pDotationCategory, float rNbInterventionType, const MT_Vector2D* pTargetPosition )
    : PHY_ActionIndirectFire_ABC( pion, pDotationCategory, rNbInterventionType )
{
    if( pDotationCategory_ && pDotationCategory_->CanBeUsedForIndirectFire() )
    {
        const auto& effects = pDotationCategory->GetIndirectFireEffects();
        for( auto it = effects.begin(); it != effects.end(); ++ it )
        {
            MIL_Effect_IndirectFire* pEffect = new MIL_Effect_IndirectFire( pion, *pTargetPosition, **it, rNbInterventionType_, 0 );
            pEffect->IncRef();
            MIL_EffectManager::GetEffectManager().Register( *pEffect );
            effects_.push_back( pEffect );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position constructor
// Created: LDC 2014-06-19
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_Position::PHY_ActionIndirectFire_Position( MIL_Agent_ABC& pion, const PHY_DotationCategory* pDotationCategory, float rNbInterventionType, const MT_Vector2D* pTargetPosition, DEC_Decision_ABC* requester )
    : PHY_ActionIndirectFire_ABC( pion, pDotationCategory, rNbInterventionType )
{
    if( pDotationCategory_ && pDotationCategory_->CanBeUsedForIndirectFire() )
    {
        const auto& effects = pDotationCategory->GetIndirectFireEffects();
        for( auto it = effects.begin(); it != effects.end(); ++ it )
        {
            MIL_Effect_IndirectFire* pEffect = new MIL_Effect_IndirectFire( pion, *pTargetPosition, **it, rNbInterventionType_, requester ? &requester->GetPion() : 0 );
            pEffect->IncRef();
            MIL_EffectManager::GetEffectManager().Register( *pEffect );
            effects_.push_back( pEffect );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_Position::~PHY_ActionIndirectFire_Position()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionIndirectFire_Position::StopAction()
{
    for( auto it = effects_.begin(); it != effects_.end(); ++it )
        (*it)->DecRef();
    PHY_ActionIndirectFire_ABC::StopAction();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionIndirectFire_Position::Execute()
{
    int nResult = PHY_RoleAction_IndirectFiring::eRunning;
    for( auto it = effects_.begin(); it != effects_.end(); ++it )
        nResult = role_.Fire( *it );
    Callback( nResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position::ExecuteSuspended
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_ActionIndirectFire_Position::ExecuteSuspended()
{
    role_.FireSuspended();
}
