// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgePopulationFunctions.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 13:57 $
// $Revision: 9 $
// $Workfile: DEC_KnowledgePopulationFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgePopulationFunctions.h"

#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Recon
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::Recon( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), callerAgent.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    pKnowledge->Recon();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::GetDangerosity( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );

    // For DIA, the dangerosity value is 1 <= dangerosity <= 2
    const MT_Float rDangerosity = pKnowledge->GetDangerosity( caller );
    call.GetResult().SetValue( (float)( rDangerosity + 1. ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Secure
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::Secure( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    pKnowledge->Secure( caller );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::SecuringPoint
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::SecuringPoint( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );

    MT_Vector2D* pResult = new MT_Vector2D( pKnowledge->GetSecuringPoint( caller ) );

    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::ClosestPoint
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::ClosestPoint( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );

    MT_Vector2D* pResult = new MT_Vector2D( pKnowledge->GetClosestPoint( caller.GetRole< PHY_RolePion_Location >().GetPosition() ) ); //$$$ RAM

    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsEnemy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::IsEnemy( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    
    call.GetResult().SetValue( caller.GetArmy().IsAnEnemy( pKnowledge->GetArmy() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::Exterminate
// Created: SBO 2005-12-22
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::Exterminate( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 2 ).SetValue( eQueryInvalid );
        return;
    }
    call.GetParameter( 2 ).SetValue( eQueryValid );
    const MT_Float rSurface = call.GetParameter( 1 ).ToFloat();
    pKnowledge->Exterminate( caller, rSurface );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsPerceivedByAgent
// Created: SBO 2006-01-18
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::IsPerceivedByAgent( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( false );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    bool bIsPerceived = caller.GetKnowledge().IsPerceived( pKnowledge->GetPopulationKnown() );
    call.GetResult().SetValue( bIsPerceived );
}
