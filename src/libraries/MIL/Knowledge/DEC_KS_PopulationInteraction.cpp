// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_PopulationInteraction.cpp $
// $Author: Jvt $
// $Modtime: 5/04/05 13:34 $
// $Revision: 4 $
// $Workfile: DEC_KS_PopulationInteraction.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_PopulationInteraction.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_PopulationInteraction::DEC_KS_PopulationInteraction( DEC_KnowledgeBlackBoard& blackBoard, const MIL_AgentPion& agentInteracting )
    : DEC_KnowledgeSource_ABC( blackBoard )
    , pAgentInteracting_     ( &agentInteracting )
{
    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_PopulationInteraction::~DEC_KS_PopulationInteraction()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );    
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::Prepare()
{
    pBlackBoard_->ApplyOnKnowledgesPopulationCollision( std::mem_fun_ref( &DEC_Knowledge_PopulationCollision::Prepare ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::GetKnowledgePopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision& DEC_KS_PopulationInteraction::GetKnowledgePopulationCollision( MIL_Population& population ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_PopulationCollision* pKnowledge = pBlackBoard_->GetKnowledgePopulationCollision( population );
    if( !pKnowledge )
        pKnowledge = &pBlackBoard_->CreateKnowledgePopulationCollision( *pAgentInteracting_, population );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::UpdateObjectPerceptionKnowledge
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
/*DEC_Knowledge_ObjectPerception& DEC_KS_PopulationInteraction::GetKnowledgeObjectPerception( MIL_RealObject_ABC& object ) const
{

}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::UpdateObjectCollisionKnowledge
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision& DEC_KS_PopulationInteraction::GetKnowledgeObjectCollision( MIL_RealObject_ABC& object ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_ObjectCollision* pKnowledge = pBlackBoard_->GetKnowledgeObjectCollision( object );
    if( !pKnowledge )
        pKnowledge = &pBlackBoard_->CreateKnowledgeObjectCollision( GetAgentInteracting(), object );
    return *pKnowledge;
}
*/

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::Talk()
{       
    // Collisions
    for( CIT_PopulationFlowVector it = flowCollisions_.begin(); it != flowCollisions_.end(); ++it )
    {
        MIL_PopulationFlow& flow = **it;
        if( !flow.IsValid() )
            continue;
        GetKnowledgePopulationCollision( flow.GetPopulation() ).Update( flow );
    }
    for( CIT_PopulationConcentrationVector it = concentrationCollisions_.begin(); it != concentrationCollisions_.end(); ++it )
    {
        MIL_PopulationConcentration& concentration = **it;
        if( !concentration.IsValid() )
            continue;
        GetKnowledgePopulationCollision( concentration.GetPopulation() ).Update( concentration );
    }

    flowCollisions_         .clear();
    concentrationCollisions_.clear();
}

// =============================================================================
// CLEAN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::CleanKnowledgePopulationCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::CleanKnowledgePopulationCollision( DEC_Knowledge_PopulationCollision& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->DestroyKnowledgePopulationCollision( knowledge ); // NB - The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::Clean
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::Clean()
{
    // Remove all invalid knowledges
    assert( pBlackBoard_ );
    class_mem_fun_void_t< DEC_KS_PopulationInteraction, DEC_Knowledge_PopulationCollision > method( DEC_KS_PopulationInteraction::CleanKnowledgePopulationCollision, *this );        
    pBlackBoard_->ApplyOnKnowledgesPopulationCollision( method );    
}
