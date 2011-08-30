// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgePopulation.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 19:05 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgePopulation.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "DEC_Knowledge_Population.h"
#include "KnowledgesVisitor_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "MT_Tools/MT_ScipioException.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgePopulation )
BOOST_CLASS_TRACKING( DEC_BlackBoard_CanContainKnowledgePopulation, boost::serialization::track_never )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulation::DEC_BlackBoard_CanContainKnowledgePopulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulation::~DEC_BlackBoard_CanContainKnowledgePopulation()
{
    while( !knowledgePopulationMap_.empty() )
        DestroyKnowledgePopulation( *knowledgePopulationMap_.begin()->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t nNbr;
    file >> nNbr;
    while ( nNbr-- )
    {
        MIL_Population* pPopulation;
        file >> pPopulation;
        file >> knowledgePopulationMap_[ pPopulation ];
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const std::size_t size = knowledgePopulationMap_.size();
    file << size;
    for ( CIT_KnowledgePopulationMap it = knowledgePopulationMap_.begin(); it != knowledgePopulationMap_.end(); ++it )
        file << it->first
             << it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::CreateKnowledgePopulation
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& DEC_BlackBoard_CanContainKnowledgePopulation::CreateKnowledgePopulation( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Population& populationPerceived )
{
    DEC_Knowledge_Population* pKnowledge = new DEC_Knowledge_Population( knowledgeGroup, populationPerceived );
    if( ! knowledgePopulationMap_.insert( std::make_pair( &populationPerceived, pKnowledge ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::DestroyKnowledgePopulation
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::DestroyKnowledgePopulation( DEC_Knowledge_Population& knowledge )
{
    if( knowledgePopulationMap_.erase( &knowledge.GetPopulationKnown() ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulationFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulationFromID( unsigned int nID ) const
{
    for( CIT_KnowledgePopulationMap it = knowledgePopulationMap_.begin(); it != knowledgePopulationMap_.end(); ++it )
    {
        DEC_Knowledge_Population& knowledge = *it->second;
        if( knowledge.GetID() == nID )
            return &knowledge;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgesPopulation
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulation( const MIL_Population& associatedPopulation ) const
{
    CIT_KnowledgePopulationMap itKnowledge = knowledgePopulationMap_.find( &associatedPopulation );
    if( itKnowledge != knowledgePopulationMap_.end() )
        return itKnowledge->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::Accept
// Created: LOIC 2011-08-29
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    visitor.VisitKnowledgesPopulation( knowledgePopulationMap_.size() );
}
