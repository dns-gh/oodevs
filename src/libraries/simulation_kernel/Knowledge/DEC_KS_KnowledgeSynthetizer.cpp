// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_KnowledgeSynthetizer.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_Population.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_KnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_KnowledgeSynthetizer::DEC_KS_KnowledgeSynthetizer( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_( &blackBoard )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_KnowledgeSynthetizer::DEC_KS_KnowledgeSynthetizer()
    : pBlackBoard_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_KnowledgeSynthetizer::~DEC_KS_KnowledgeSynthetizer()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::Prepare()
{
    // Agent
    std::function< void( DEC_Knowledge_Agent& ) > agentFunctor = boost::bind( & DEC_Knowledge_Agent::Prepare, _1 );
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( agentFunctor );

    // Population
    std::function< void( DEC_Knowledge_Population& ) > populationFunctor = boost::bind( & DEC_Knowledge_Population::Prepare, _1 );
    pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( populationFunctor );
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
    std::function< void( DEC_Knowledge_Agent& ) > agentFunctorExtrapolate = boost::bind( & DEC_Knowledge_Agent::Extrapolate, _1 );
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( agentFunctorExtrapolate );
    // Relevance
    std::function< void( DEC_Knowledge_Agent& ) > agentFunctorRelevance = boost::bind( &DEC_Knowledge_Agent::UpdateRelevance, _1, currentTimeStep );
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( agentFunctorRelevance );
    std::function< void( DEC_Knowledge_Population& ) > populationFunctor = boost::bind( &DEC_Knowledge_Population::UpdateRelevance, _1 );
    pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( populationFunctor );
}

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
// Name: DEC_KS_KnowledgeSynthetizer::CleanKnowledgeDeletedAgent
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::CleanKnowledgeDeletedAgent( DEC_Knowledge_Agent& knowledge )
{
    assert( pBlackBoard_ );
    if( knowledge.GetAgentKnown().IsMarkedForDestruction() )
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
// Name: DEC_KS_KnowledgeSynthetizer::CleanKnowledgeObject
// Created: JSR 2010-07-01
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::CleanKnowledgeObject( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge )
{
    if( knowledge->Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgeObjectContainer().DestroyKnowledgeObject( *knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::Clean()
{
    assert( pBlackBoard_ );

    auto methodAgent = boost::bind( &DEC_KS_KnowledgeSynthetizer::CleanKnowledgeAgent, this, _1 );
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( methodAgent );

    auto methodPopulation = boost::bind( &DEC_KS_KnowledgeSynthetizer::CleanKnowledgePopulation, this, _1 );
    pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( methodPopulation );

    auto methodObject = boost::bind( &DEC_KS_KnowledgeSynthetizer::CleanKnowledgeObject, this, _1 );
    pBlackBoard_->GetKnowledgeObjectContainer().ApplyOnKnowledgesObject( methodObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeSynthetizer::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeSynthetizer::CleanDeletedAgentKnowledges()
{
    auto methodAgent = boost::bind( &DEC_KS_KnowledgeSynthetizer::CleanKnowledgeDeletedAgent, this, _1 );
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( methodAgent );
}
