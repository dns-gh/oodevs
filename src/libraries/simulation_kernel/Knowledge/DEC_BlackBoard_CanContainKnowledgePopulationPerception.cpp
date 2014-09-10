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

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "Entities/Populations/MIL_Population.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgePopulationPerception )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulationPerception::DEC_BlackBoard_CanContainKnowledgePopulationPerception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulationPerception::~DEC_BlackBoard_CanContainKnowledgePopulationPerception()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t nNbr;
    file >> nNbr;
    while( nNbr-- )
    {
        MIL_Population* pPopulation;
        file >> pPopulation;
        file >> perceptions_[ pPopulation ];
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    std::size_t size = perceptions_.size();
    file << size;
    for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
    {
        file << it->first;
        file << it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::CreateKnowledgePopulationPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception& DEC_BlackBoard_CanContainKnowledgePopulationPerception::CreateKnowledgePopulationPerception( const MIL_Agent_ABC& agentPerceiving, MIL_Population& populationPerceived )
{
    DEC_Knowledge_PopulationPerception* pKnowledge = new DEC_Knowledge_PopulationPerception( agentPerceiving, populationPerceived );//$$ RAM
    if( ! perceptions_.insert( std::make_pair( &populationPerceived, pKnowledge ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::DestroyKnowledgePopulationPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationPerception::DestroyKnowledgePopulationPerception( DEC_Knowledge_PopulationPerception& knowledge )
{
    if( perceptions_.erase( &knowledge.GetPopulationPerceived() ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::Clear
// Created: LDC 2014-09-08
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulationPerception::Clear()
{
    while( !perceptions_.empty() )
        DestroyKnowledgePopulationPerception( *perceptions_.begin()->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulationPerception::GetKnowledgesPopulationPerception
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception* DEC_BlackBoard_CanContainKnowledgePopulationPerception::GetKnowledgePopulationPerception( const MIL_Population& associatedPopulation ) const
{
    auto it = perceptions_.find( &associatedPopulation );
    if( it != perceptions_.end() )
        return it->second;
    else
        return 0;
}
