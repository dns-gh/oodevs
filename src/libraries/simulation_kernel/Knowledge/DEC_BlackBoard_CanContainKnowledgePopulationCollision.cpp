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
#include "MT_Tools/MT_Logger.h"

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
    Clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t nNbr;
    file >> nNbr;
    while( nNbr-- )
    {
        MIL_Population* pPopulation;
        file >> pPopulation;
        file >> collisions_[ pPopulation ];
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    std::size_t size = collisions_.size();
    file << size;
    for( auto it = collisions_.begin(); it != collisions_.end(); ++it )
    {
        file << it->first;
        file << it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::CreateKnowledgePopulationCollision
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision& DEC_BlackBoard_CanContainKnowledgePopulationCollision::CreateKnowledgePopulationCollision( const MIL_Agent_ABC& agent, MIL_Population& population )
{
    DEC_Knowledge_PopulationCollision* pKnowledge = new DEC_Knowledge_PopulationCollision( agent, population );//$$ RAM
    if( ! collisions_.insert( std::make_pair( &population, pKnowledge ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::DestroyKnowledgePopulationCollision
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::DestroyKnowledgePopulationCollision( DEC_Knowledge_PopulationCollision& knowledge )
{
    if( collisions_.erase( &knowledge.GetPopulation() ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::Clear
// Created: LDC 2014-09-08
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::Clear()
{
    while( !collisions_.empty() )
        DestroyKnowledgePopulationCollision( *collisions_.begin()->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgesPopulationCollision
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationCollision* DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgePopulationCollision( const MIL_Population& associatedPopulation ) const
{
    auto it = collisions_.find( &associatedPopulation );
    if( it != collisions_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgesPopulationCollision
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationCollision::GetKnowledgesPopulationCollision( T_KnowledgePopulationCollisionVector& container ) const
{
    container.clear(); container.reserve( collisions_.size() );
    for( auto it = collisions_.begin(); it != collisions_.end(); ++it )
        container.push_back( it->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationCollision::HasCollision
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
bool DEC_BlackBoard_CanContainKnowledgePopulationCollision::HasCollision() const
{
    return !collisions_.empty();
}