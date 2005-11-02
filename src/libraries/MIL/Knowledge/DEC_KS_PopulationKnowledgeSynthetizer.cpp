// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_PopulationKnowledgeSynthetizer.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 11:01 $
// $Revision: 7 $
// $Workfile: DEC_KS_PopulationKnowledgeSynthetizer.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_PopulationKnowledgeSynthetizer.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "DEC_Knowledge_Population.h"

#include "MIL_KnowledgeGroup.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
 
// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_PopulationKnowledgeSynthetizer::DEC_KS_PopulationKnowledgeSynthetizer( DEC_KnowledgeBlackBoard& blackBoard, const MIL_KnowledgeGroup& knowledgeGroup )
    : DEC_KnowledgeSource_ABC( blackBoard )
    , pKnowledgeGroup_       ( &knowledgeGroup )
{
    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this );       
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_PopulationKnowledgeSynthetizer::~DEC_KS_PopulationKnowledgeSynthetizer()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::Prepare()
{
    pBlackBoard_->ApplyOnKnowledgesPopulation( std::mem_fun_ref( & DEC_Knowledge_Population::Prepare ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::GetKnowledgeToUpdate
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Population& DEC_KS_PopulationKnowledgeSynthetizer::GetKnowledgeToUpdate( MIL_Population& populationKnown ) const
{
    assert( pBlackBoard_ );
    DEC_Knowledge_Population* pKnowledge = pBlackBoard_->GetKnowledgePopulation( populationKnown );
    if( pKnowledge )
        return *pKnowledge;
    
    assert( pKnowledgeGroup_ );
    return pBlackBoard_->CreateKnowledgePopulation( *pKnowledgeGroup_, populationKnown );
}
  
// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
void DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromPerception( const DEC_Knowledge_PopulationPerception& perception )
{
    GetKnowledgeToUpdate( perception.GetPopulationPerceived() ).Update( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromCollision
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromCollision( const DEC_Knowledge_PopulationCollision& collision )
{
    GetKnowledgeToUpdate( collision.GetPopulation() ).Update( collision );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::Talk
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::Talk()
{
    assert( pKnowledgeGroup_ );
    assert( pBlackBoard_ );

    class_mem_fun_void_const_t< DEC_KS_PopulationKnowledgeSynthetizer, DEC_Knowledge_PopulationPerception > methodPerception( DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromPerception, *this );
    class_mem_fun_void_const_t< DEC_KS_PopulationKnowledgeSynthetizer, DEC_Knowledge_PopulationCollision  > methodCollision ( DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromCollision , *this );

    // Synthèse de la perception des subordonnés
    // Ajout automatique de la connaissance de chaque subordonné    
    const MIL_KnowledgeGroup::T_AutomateVector& automates = pKnowledgeGroup_->GetAutomates();
    for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
    {
        const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        {
            MIL_AgentPion& pion = **itPion;
            pion.GetKnowledge().ApplyOnKnowledgesPopulationPerception( methodPerception );
            pion.GetKnowledge().ApplyOnKnowledgesPopulationCollision ( methodCollision  );
        }
    }
    
    // Relevance
    pBlackBoard_->ApplyOnKnowledgesPopulation( std::mem_fun_ref( & DEC_Knowledge_Population::UpdateRelevance ) );
}

// =============================================================================
// CLEAN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::CleanKnowledgePopulation
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::CleanKnowledgePopulation( DEC_Knowledge_Population& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->DestroyKnowledgePopulation( knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::Clean()
{
    assert( pBlackBoard_ );

    class_mem_fun_void_t< DEC_KS_PopulationKnowledgeSynthetizer, DEC_Knowledge_Population > methodPopulation( DEC_KS_PopulationKnowledgeSynthetizer::CleanKnowledgePopulation, *this );
    pBlackBoard_->ApplyOnKnowledgesPopulation( methodPopulation );
}