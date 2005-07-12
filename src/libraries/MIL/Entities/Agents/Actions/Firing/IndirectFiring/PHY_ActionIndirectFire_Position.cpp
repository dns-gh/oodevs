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

#include "MIL_Pch.h"

#include "PHY_ActionIndirectFire_Position.h"
#include "PHY_RoleAction_IndirectFiring.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_Position::PHY_ActionIndirectFire_Position( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_ActionIndirectFire_ABC( pion, diaCall )
{
    assert( DEC_Tools::CheckTypePoint( diaCall.GetParameter( 3 ) ) );

    MT_Vector2D* pTargetPosition = diaCall.GetParameter( 3 ).ToUserPtr( pTargetPosition );
    assert( pTargetPosition );
    pEffect_ = new MIL_Effect_IndirectFire( pion_, *pTargetPosition, *pIndirectWeaponClass_, rNbInterventionType_ );
    pEffect_->IncRef();
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( *pEffect_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionIndirectFire_Position::~PHY_ActionIndirectFire_Position()
{
    assert( pEffect_ );
    pEffect_->ForceFlying();
    pEffect_->DecRef();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionIndirectFire_Position::Execute()
{
    assert( pEffect_ );
    int nResult = role_.Fire( *pEffect_ );
    diaReturnCode_.SetValue( nResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIndirectFire_Position::ExecuteSuspended
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_ActionIndirectFire_Position::ExecuteSuspended()
{
    role_.FireSuspended();
}
