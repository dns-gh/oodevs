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
#include "MT_Tools/MT_ScipioException.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgePopulationCollision )

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
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t nNbr;
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
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    std::size_t size = knowledgePopulationCollisionMap_.size();
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
DEC_Knowledge_PopulationCollision& DEC_BlackBoard_CanContainKnowledgePopulationCollision::CreateKnowledgePopulationCollision( const MIL_Agent_ABC& agent, MIL_Population& population )
{
    DEC_Knowledge_PopulationCollision* pKnowledge = new DEC_Knowledge_PopulationCollision( agent, population );//$$ RAM
    if( ! knowledgePopulationCollisionMap_.insert( std::make_pair( &population, pKnowledge ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::DestroyKnowledgePopulationCollision
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::DestroyKnowledgePopulationCollision( DEC_Knowledge_PopulationCollision& knowledge )
{
    if( knowledgePopulationCollisionMap_.erase( &knowledge.GetPopulation() ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
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

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::HasCollision
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
bool DEC_BlackBoard_CanContainKnowledgePopulationCollision::HasCollision() const
{
    return !knowledgePopulationCollisionMap_.empty();
}