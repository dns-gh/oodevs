// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 19:05 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgePopulationPerception.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"

#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_PopulationPerception.h"

#include "Entities/Populations/MIL_Population.h"

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgePopulationPerception, "DEC_BlackBoard_CanContainKnowledgePopulationPerception" )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulationPerception::DEC_BlackBoard_CanContainKnowledgePopulationPerception()
{
    
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulationPerception::~DEC_BlackBoard_CanContainKnowledgePopulationPerception()
{
    while( !knowledgePopulationPerceptionMap_.empty() )
        DestroyKnowledgePopulationPerception( *knowledgePopulationPerceptionMap_.begin()->second );        
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationPerception::load( MIL_CheckPointInArchive& file, const uint )
{
//    uint nNbr;
//    file >> nNbr;
//    while ( nNbr-- )
//    {
//        MIL_Population* pPopulation;
//        file >> pPopulation;
//        file >> knowledgePopulationPerceptionMap_[ pPopulation ];
//    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
//    file << knowledgePopulationPerceptionMap_.size();
//    for ( CIT_KnowledgePopulationPerceptionMap it = knowledgePopulationPerceptionMap_.begin(); it != knowledgePopulationPerceptionMap_.end(); ++it )
//    {
//        file << it->first
//             << it->second;
//    }
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::CreateKnowledgePopulationPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception& DEC_BlackBoard_CanContainKnowledgePopulationPerception::CreateKnowledgePopulationPerception( const MIL_AgentPion& agentPerceiving, MIL_Population& populationPerceived )
{
    DEC_Knowledge_PopulationPerception* pKnowledge = new DEC_Knowledge_PopulationPerception( agentPerceiving, populationPerceived );//$$ RAM   
    bool bOut = knowledgePopulationPerceptionMap_.insert( std::make_pair( &populationPerceived, pKnowledge ) ).second;
    assert( bOut );
    return *pKnowledge;
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::DestroyKnowledgePopulationPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationPerception::DestroyKnowledgePopulationPerception( DEC_Knowledge_PopulationPerception& knowledge )
{
    int nOut = knowledgePopulationPerceptionMap_.erase( &knowledge.GetPopulationPerceived() );
    assert( nOut == 1 );
    delete &knowledge;
}

