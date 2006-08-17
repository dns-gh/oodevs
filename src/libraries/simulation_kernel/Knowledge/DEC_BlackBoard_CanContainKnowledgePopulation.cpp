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

#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_Population.h"

#include "Entities/Populations/MIL_Population.h"

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgePopulation, "DEC_BlackBoard_CanContainKnowledgePopulation" )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulation::DEC_BlackBoard_CanContainKnowledgePopulation()
{
    
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

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nNbr;
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
void DEC_BlackBoard_CanContainKnowledgePopulation::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << knowledgePopulationMap_.size();
    for ( CIT_KnowledgePopulationMap it = knowledgePopulationMap_.begin(); it != knowledgePopulationMap_.end(); ++it )
    {
        file << it->first
             << it->second;
    }
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::CreateKnowledgePopulation
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& DEC_BlackBoard_CanContainKnowledgePopulation::CreateKnowledgePopulation( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Population& populationPerceived )
{
    DEC_Knowledge_Population* pKnowledge = new DEC_Knowledge_Population( knowledgeGroup, populationPerceived );
    bool bOut = knowledgePopulationMap_.insert( std::make_pair( &populationPerceived, pKnowledge ) ).second;
    assert( bOut );
    return *pKnowledge;
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::DestroyKnowledgePopulation
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::DestroyKnowledgePopulation( DEC_Knowledge_Population& knowledge )
{
    int nOut = knowledgePopulationMap_.erase( &knowledge.GetPopulationKnown() );
    assert( nOut == 1 );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulationFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulationFromID( uint nID ) const
{
    for( CIT_KnowledgePopulationMap it = knowledgePopulationMap_.begin(); it != knowledgePopulationMap_.end(); ++it )
    {
        DEC_Knowledge_Population& knowledge = *it->second;
        if( knowledge.GetID() == nID ) 
            return &knowledge;
    }
    return 0;
}
