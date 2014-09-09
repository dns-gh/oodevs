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

#include "simulation_kernel_pch.h"
#include "DEC_KS_PopulationInteraction.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationCollision.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_PopulationInteraction )

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_PopulationInteraction::DEC_KS_PopulationInteraction( DEC_KnowledgeBlackBoard_AgentPion& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_( &blackBoard )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction constructor
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
DEC_KS_PopulationInteraction::DEC_KS_PopulationInteraction()
    : pBlackBoard_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_PopulationInteraction::~DEC_KS_PopulationInteraction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KS_PopulationInteraction::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_PopulationInteraction::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
            & pBlackBoard_
            & concentrationCollisions_
            & flowCollisions_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::Prepare()
{
    assert( pBlackBoard_ );
    std::mem_fun_ref_t< void, DEC_Knowledge_PopulationCollision > populationFunctor = std::mem_fun_ref( &DEC_Knowledge_PopulationCollision::Prepare );
    pBlackBoard_->GetKnowledgePopulationCollisionContainer().ApplyOnKnowledgesPopulationCollision( populationFunctor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::GetKnowledgePopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision& DEC_KS_PopulationInteraction::GetKnowledgePopulationCollision( MIL_Population& population ) const
{
    assert( pBlackBoard_ );

    DEC_Knowledge_PopulationCollision* pKnowledge = pBlackBoard_->GetKnowledgePopulationCollisionContainer().GetKnowledgePopulationCollision( population );
    if( !pKnowledge )
        pKnowledge = &pBlackBoard_->GetKnowledgePopulationCollisionContainer().CreateKnowledgePopulationCollision( pBlackBoard_->GetPion(), population );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::Talk( int /*currentTimeStep*/ )
{
    // Collisions
    for( auto it = flowCollisions_.begin(); it != flowCollisions_.end(); ++it )
    {
        MIL_PopulationFlow& flow = **it;
        if( !flow.IsValid() )
            continue;
        GetKnowledgePopulationCollision( flow.GetPopulation() ).Update( flow );
    }
    for( auto it = concentrationCollisions_.begin(); it != concentrationCollisions_.end(); ++it )
    {
        MIL_PopulationConcentration& concentration = **it;
        if( !concentration.IsValid() )
            continue;
        GetKnowledgePopulationCollision( concentration.GetPopulation() ).Update( concentration );
    }

    flowCollisions_         .clear();
    concentrationCollisions_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::CleanKnowledgePopulationCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::CleanKnowledgePopulationCollision( DEC_Knowledge_PopulationCollision& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgePopulationCollisionContainer().DestroyKnowledgePopulationCollision( knowledge ); // NB - The knowledge will be deleted
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
    auto method = boost::bind( &DEC_KS_PopulationInteraction::CleanKnowledgePopulationCollision, this, _1 );
    pBlackBoard_->GetKnowledgePopulationCollisionContainer().ApplyOnKnowledgesPopulationCollision( method );
}


// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::Clear
// Created: LDC 2014-09-09
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::Clear()
{
    flowCollisions_.clear();
    concentrationCollisions_.clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::CleanDeletedAgentKnowledges()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::NotifyPopulationCollision( MIL_PopulationFlow& flow )
{
    flowCollisions_.push_back( &flow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationInteraction::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_KS_PopulationInteraction::NotifyPopulationCollision( MIL_PopulationConcentration& concentration )
{
    concentrationCollisions_.push_back( &concentration );
}
