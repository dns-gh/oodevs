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

#include "MIL_pch.h"
#include "DEC_KnowledgePopulationFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_Knowledge_Population.h"

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
// Name: DEC_KnowledgePopulationFunctions::ClosestPoint
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
void DEC_KnowledgePopulationFunctions::ClosestPoint( DIA_Call_ABC& call, const MIL_AgentPion& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (void*)0, DEC_Tools::GetTypePoint() );GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }
    
    call.GetParameter( 1 ).SetValue( eQueryValid );
    const TER_Localisation* pLoc = call.GetParameter( 1 ).ToUserPtr( pLoc );
    assert( pLoc );
    call.GetResult().SetValue( pKnowledge->IsInZone( *pLoc ) );
}
