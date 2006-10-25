// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeAgentFunctions.cpp $
// $Author: Jvt $
// $Modtime: 23/05/05 16:32 $
// $Revision: 8 $
// $Workfile: DEC_KnowledgeAgentFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeAgentFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Units/Categories/PHY_NatureAtlas.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Decision/DEC_Tools.h"
#include "MT_Tools/MT_Sector.h"

// =============================================================================
// FUNCTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetNatureAtlas
// Created: NLD 2004-03-31
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::GetNatureAtlas( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (int)pKnowledge->GetNatureAtlas().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetDangerosity
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::GetDangerosity( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );

    // For DIA, the dangerosity value is 1 <= dangerosity <= 2
    const MT_Float rDangerosity = pKnowledge->GetDangerosity( callerAgent );
    call.GetResult().SetValue( (float)( rDangerosity + 1. ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsInSameDirection
// Created: JVT 2004-12-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::IsInSameDirection( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 1 ) ) );

    const DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );

    if ( !pKnowledge )
    {
        call.GetResult().SetValue( false );
        return;
    }

    MT_Vector2D* pDirection = call.GetParameter( 1 ).ToUserPtr( pDirection );
    assert( pDirection );

    const MT_Vector2D& vOrigin = callerAgent.GetRole< PHY_RolePion_Location >().GetPosition();

    call.GetResult().SetValue( *pDirection * ( pKnowledge->GetPosition() - vOrigin ) > 0. );
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsInDetectionCone
// Created: JVT 2005-08-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::IsInDetectionCone( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeDirection( call.GetParameter( 1 ) ) );

    const DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    
    if ( !pKnowledge )
    {
        call.GetResult().SetValue( false );
        return;
    }
        
    const MT_Vector2D& vOrigin    = callerAgent.GetRole< PHY_RolePion_Location >().GetPosition();
          MT_Vector2D* pDirection = call.GetParameter( 1 ).ToUserPtr( pDirection );
    const MT_Float     rAngle     = call.GetParameter( 2 ).ToFloat() * MT_PI / 180.;
    const MT_Float     rDist      = callerAgent.GetRole< PHY_RolePion_Perceiver >().GetMaxAgentPerceptionDistance();
    
    assert( pDirection );

    call.GetResult().SetValue( MT_Sector( vOrigin, *pDirection, rAngle ).IsInCone( pKnowledge->GetPosition(), rDist ) );
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPerceivedByAgent
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::IsPerceivedByAgent( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    bool bIsPerceived = callerAgent.GetKnowledge().IsPerceived( pKnowledge->GetAgentKnown() );
    call.GetResult().SetValue( bIsPerceived );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPerceivedByKnowledgeGroup
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::IsPerceivedByKnowledgeGroup( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( ( pKnowledge->GetCurrentPerceptionLevel() != PHY_PerceptionLevel::notSeen_ ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsAnEnemy
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::IsAnEnemy( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( callerAgent.GetArmy().IsAnEnemy( *pKnowledge ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsMoving
// Created: NLD 2004-04-09
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::IsMoving( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    const DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (bool)( pKnowledge->GetSpeed() != 0. ) );
}
    

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPerceivingAgent
// Created: NLD 2004-05-17
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::IsPerceivingAgent( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    //$$$ Fonction BOF : trop de triche ...
    const DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    
    bool bResult = pKnowledge->GetAgentKnown().IsPerceived( callerAgent );
    call.GetResult().SetValue( bResult );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::KillOfficers
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::KillOfficers( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    pKnowledge->KillOfficers();
}
