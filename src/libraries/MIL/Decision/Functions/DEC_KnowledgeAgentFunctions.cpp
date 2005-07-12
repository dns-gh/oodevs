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

#include "MIL_pch.h"
#include "DEC_KnowledgeAgentFunctions.h"

#include "Knowledge/DEC_KS_AgentQuerier.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/MIL_Army.h"

#include "Entities/Agents/Units/Categories/PHY_NatureWeapon.h"
#include "Entities/Agents/Units/Categories/PHY_NatureAtlas.h"

// =============================================================================
// FUNCTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetEtatOps
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::GetEtatOps( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (float)pKnowledge->GetEtatOps() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetNatureWeapon
// Created: NLD 2004-03-31
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::GetNatureWeapon( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (int)pKnowledge->GetNatureWeapon().GetID() );
}

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
// Name: DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
     if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (int)pKnowledge->GetMaxPerceptionLevel().GetID() );
}
// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPerceivedByAgent
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::IsPerceivedByAgent( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    bool bIsPerceived = callerAgent.GetKSQuerier().IsAgentPerceived( pKnowledge->GetAgentKnown() );
    call.GetResult().SetValue( bIsPerceived );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPerceivedByKnowledgeGroup
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::IsPerceivedByKnowledgeGroup( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
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
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
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
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
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
    DEC_Knowledge_Agent* pKnowledge = DEC_FunctionsTools::GetKnowledgeAgentFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    
    bool bResult = pKnowledge->GetAgentKnown().IsAgentPerceived( callerAgent );
    call.GetResult().SetValue( bResult );
}

