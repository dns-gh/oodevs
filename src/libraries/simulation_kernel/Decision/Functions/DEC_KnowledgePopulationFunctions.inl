// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_KnowledgePopulationFunctions.inl $
// $Author: Nld $
// $Modtime: 16/03/05 10:58 $
// $Revision: 9 $
// $Workfile: DEC_KnowledgePopulationFunctions.inl $
//
// *****************************************************************************

#include "DEC_FunctionsTools.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgePopulationFunctions::GetDominationState( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (float)0. );
        return;
    }

    call.GetParameter( 1 ).SetValue( eQueryValid );
    call.GetResult().SetValue( (float)pKnowledge->GetDominationState() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetAttitude
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgePopulationFunctions::GetAttitude( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
        call.GetResult().SetValue( (int)0 );
    else
        call.GetResult().SetValue( (int)pKnowledge->GetAttitude().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsRecon
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgePopulationFunctions::IsRecon( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
        call.GetResult().SetValue( false );
    else
        call.GetResult().SetValue( pKnowledge->IsRecon() );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgePopulationFunctions::IsInZone
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_KnowledgePopulationFunctions::IsInZone( DIA_Call_ABC& call, const T& caller )
{
    DEC_Knowledge_Population* pKnowledge = DEC_FunctionsTools::GetKnowledgePopulationFromDia( call.GetParameter( 0 ), caller.GetKnowledgeGroup() );
    if( !pKnowledge )
    {
        call.GetResult().SetValue( false );
        return;
    }
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 1 ) ) );
    const TER_Localisation* pLoc = call.GetParameter( 1 ).ToUserPtr( pLoc );
    assert( pLoc );
    call.GetResult().SetValue( pKnowledge->IsInZone( *pLoc ) );
}
