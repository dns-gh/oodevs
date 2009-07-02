// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationCollision.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:28 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgePopulationCollision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationCollision.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "Entities/Populations/MIL_Population.h"

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgePopulationCollision, "DEC_BlackBoard_CanContainKnowledgePopulationCollision" )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulationCollision::DEC_BlackBoard_CanContainKnowledgePopulationCollision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulationCollision::~DEC_BlackBoard_CanContainKnowledgePopulationCollision()
{
    while ( !knowledgePopulationCollisionMap_.empty() )
        DestroyKnowledgePopulationCollision( *knowledgePopulationCollisionMap_.begin()->second );        
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nNbr;
    file >> nNbr;
    while ( nNbr-- )
    {
        MIL_Population* pPopulation;
        file >> pPopulation;
        file >> knowledgePopulationCollisionMap_[ pPopulation ];
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    uint size = knowledgePopulationCollisionMap_.size();
    file << size;
    for ( CIT_KnowledgePopulationCollisionMap it = knowledgePopulationCollisionMap_.begin(); it != knowledgePopulationCollisionMap_.end(); ++it )
    {
        file << it->first
             << it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::CreateKnowledgePopulationCollision
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision& DEC_BlackBoard_CanContainKnowledgePopulationCollision::CreateKnowledgePopulationCollision( const MIL_AgentPion& agent, MIL_Population& population )
{
    DEC_Knowledge_PopulationCollision* pKnowledge = new DEC_Knowledge_PopulationCollision( agent, population );//$$ RAM   
    bool bOut = knowledgePopulationCollisionMap_.insert( std::make_pair( &population, pKnowledge ) ).second;
    assert( bOut );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::DestroyKnowledgePopulationCollision
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::DestroyKnowledgePopulationCollision( DEC_Knowledge_PopulationCollision& knowledge )
{
    int nOut = knowledgePopulationCollisionMap_.erase( &knowledge.GetPopulation() );
    assert( nOut == 1 );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgesPopulationCollision
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision* DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgePopulationCollision( const MIL_Population& associatedPopulation ) const
{
    CIT_KnowledgePopulationCollisionMap itKnowledge = knowledgePopulationCollisionMap_.find( &associatedPopulation );
    if( itKnowledge != knowledgePopulationCollisionMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgesPopulationCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgesPopulationCollision( T_KnowledgePopulationCollisionVector& container ) const
{
    container.clear(); container.reserve( knowledgePopulationCollisionMap_.size() );
    for( CIT_KnowledgePopulationCollisionMap itKnowledge = knowledgePopulationCollisionMap_.begin(); itKnowledge != knowledgePopulationCollisionMap_.end(); ++itKnowledge )
        container.push_back( itKnowledge->second );
}
