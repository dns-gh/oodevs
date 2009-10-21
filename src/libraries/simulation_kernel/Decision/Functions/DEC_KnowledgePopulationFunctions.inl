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
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
template< typename T > 
std::pair< float, int > DEC_KnowledgePopulationFunctions::GetDominationState( const T& caller, unsigned int knowledgeId )
{
	DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( !pKnowledge )
        return std::pair< float, int >( 0.f, eQueryInvalid );

    return std::pair< float, int >( pKnowledge->GetDominationState(), eQueryValid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::GetAttitude
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
template< typename T >
unsigned int DEC_KnowledgePopulationFunctions::GetAttitude( const T& caller, unsigned int knowledgeId )
{
	DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( !pKnowledge )
        return 0;
    else
        return pKnowledge->GetAttitude().GetID() ;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgePopulationFunctions::IsRecon
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
template< typename T > 
bool DEC_KnowledgePopulationFunctions::IsRecon( const T& caller, unsigned int knowledgeId )
{
	DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( !pKnowledge )
        return false ;
    else
        return pKnowledge->IsRecon();
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgePopulationFunctions::IsInZone
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
template< typename T > 
bool DEC_KnowledgePopulationFunctions::IsInZone( const T& caller, unsigned int knowledgeId, TER_Localisation* pLoc )
{
	DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(knowledgeId);
    if( !pKnowledge )
    {
        return false;
    }
    return  pKnowledge->IsInZone( *pLoc ) ;
}
