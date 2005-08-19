// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionControlZone.cpp $
// $Author: Age $
// $Modtime: 31/01/05 13:13 $
// $Revision: 5 $
// $Workfile: PHY_ActionControlZone.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ActionControlZone.h"

#include "PHY_RoleAction_DirectFiring.h"
#include "PHY_ControlZoneFireResults.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Objects/MIL_ControlZone.h"
#include "Entities/Objects/MIL_VirtualObjectType.h"
#include "Entities/MIL_EntityManager.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"
#include "MIL_AgentServer.h"
#include "TER/TER_Localisation.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionControlZone::PHY_ActionControlZone( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC    ( pion )
    , role_             ( pion.GetRole< PHY_RoleAction_DirectFiring >() )
    , pFireResult_      ( 0 )  
    , pZoneControlled_  ( 0 )
    , pPerceptionZoneID_( 0 )
{    
    DEC_Tools::CheckTypeLocalisation( diaCall.GetParameter( 0 ) );
    
    const TER_Localisation* pLocalisation  = diaCall.GetParameter( 0 ).ToUserPtr( pLocalisation );
    const MT_Float          rRadius        = MIL_Tools::ConvertMeterToSim( diaCall.GetParameter( 1 ).ToFloat() );

    
    // Fire
    if( diaCall.GetParameter( 2 ).ToBool() )
    {
        pZoneControlled_ = new MIL_ControlZone( pion.GetArmy(), *pLocalisation, rRadius );
        MIL_AgentServer::GetWorkspace().GetEntityManager().RegisterObject( *pZoneControlled_ );
    }

    // Detection
    if ( pLocalisation->GetArea() <= rRadius * rRadius * MT_PI ) 
        pPerceptionZoneID_ = pion_.GetRole< PHY_RolePion_Perceiver >().EnableControlLocalisation( *pLocalisation );
    else
        pPerceptionZoneID_ = pion_.GetRole< PHY_RolePion_Perceiver >().EnableRecoLocalisation   ( *pLocalisation, rRadius );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionControlZone::~PHY_ActionControlZone()
{
    pion_.GetRole< PHY_RolePion_Perceiver >().DisableRecoLocalisation( pPerceptionZoneID_ );

    if( pFireResult_ )
        pFireResult_->DecRef();

    if( pZoneControlled_ )
        pZoneControlled_->MarkForDestruction();

    pZoneControlled_   = 0;
    pFireResult_       = 0;
    pPerceptionZoneID_ = 0;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionControlZone::Execute()
{   
    if( !pZoneControlled_ )
        return;

    const bool bMustRefResult = ( pFireResult_ == 0 );
    role_.FireZone( *pZoneControlled_, pFireResult_ );
    if( pFireResult_ && bMustRefResult )
        pFireResult_->IncRef();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionControlZone::ExecuteSuspended()
{
    Execute();
}
