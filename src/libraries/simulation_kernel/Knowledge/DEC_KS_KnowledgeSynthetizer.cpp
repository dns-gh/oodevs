// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_KnowledgeSynthetizer.h"

#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_Population.h"
#include "MIL_KnowledgeGroup.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
 
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_KnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_KnowledgeSynthetizer::DEC_KS_KnowledgeSynthetizer( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_           ( &blackBoard )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_KnowledgeSynthetizer::DEC_KS_KnowledgeSynthetizer()
    : DEC_KnowledgeSource_ABC()
    , pBlackBoard_           ( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_KnowledgeSynthetizer::~DEC_KS_KnowledgeSynthetizer()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::Prepare()
{
    // Agent 
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( & DEC_Knowledge_Agent::Prepare, _1 ) );

    // Population
    pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( boost::bind( & DEC_Knowledge_Population::Prepare, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer::Talk
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::Talk( int currentTimeStep )
{
    assert( pBlackBoard_ );

    pBlackBoard_->ApplyOnKnowledgesPerception( currentTimeStep ); 

    // Extrapolation
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( & DEC_Knowledge_Agent::Extrapolate, _1 ) );
    // Relevance
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( &DEC_Knowledge_Agent::UpdateRelevance, _1, currentTimeStep ) );

    pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( boost::bind( &DEC_Knowledge_Population::UpdateRelevance, _1 ) );
}

// =============================================================================
// CLEAN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer::CleanKnowledgeAgent
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::CleanKnowledgeAgent( DEC_Knowledge_Agent& knowledge )
{
    assert( pBlackBoard_ );

    if( knowledge.Clean() )
        pBlackBoard_->GetKnowledgeAgentContainer().DestroyKnowledgeAgent( knowledge ); // The knowledge will be deleted
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer::CleanKnowledgePopulation
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::CleanKnowledgePopulation( DEC_Knowledge_Population& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgePopulationContainer().DestroyKnowledgePopulation( knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::Clean()
{
    assert( pBlackBoard_ );

    class_mem_fun_void_t< DEC_KS_KnowledgeSynthetizer, DEC_Knowledge_Agent > methodAgent( & DEC_KS_KnowledgeSynthetizer::CleanKnowledgeAgent, *this );
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( methodAgent );

    class_mem_fun_void_t< DEC_KS_KnowledgeSynthetizer, DEC_Knowledge_Population > methodPopulation( & DEC_KS_KnowledgeSynthetizer::CleanKnowledgePopulation, *this );
    pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( methodPopulation );
}
