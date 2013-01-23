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
#include "CheckPoints/SerializationTools.h"
#include "KnowledgesVisitor_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "MT_Tools/MT_Logger.h"

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
    while( nNbr-- )
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
    for( auto it = knowledgePopulationMap_.begin(); it != knowledgePopulationMap_.end(); ++it )
    {
        file << it->first;
        file << it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::CreateKnowledgePopulation
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& DEC_BlackBoard_CanContainKnowledgePopulation::CreateKnowledgePopulation( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup, MIL_Population& populationPerceived )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge( new DEC_Knowledge_Population( knowledgeGroup, populationPerceived ) );
    if( ! knowledgePopulationMap_.insert( std::make_pair( &populationPerceived, pKnowledge ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::DestroyKnowledgePopulation
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::DestroyKnowledgePopulation( DEC_Knowledge_Population& knowledge )
{
    if( knowledgePopulationMap_.erase( &knowledge.GetPopulationKnown() ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulationFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulationFromID( unsigned int nID ) const
{
    for( auto it = knowledgePopulationMap_.begin(); it != knowledgePopulationMap_.end(); ++it )
    {
        const boost::shared_ptr< DEC_Knowledge_Population >& knowledge = it->second;
        if( knowledge->GetID() == nID )
            return knowledge;
    }
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgesPopulation
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulation( const MIL_Population& associatedPopulation ) const
{
    auto itKnowledge = knowledgePopulationMap_.find( &associatedPopulation );
    if( itKnowledge != knowledgePopulationMap_.end() )
        return itKnowledge->second;
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulations
// Created: NPT 2012-08-09
// -----------------------------------------------------------------------------
const DEC_BlackBoard_CanContainKnowledgePopulation::T_KnowledgePopulationMap& DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulations() const
{
    return knowledgePopulationMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::Accept
// Created: LGY 2011-08-29
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    visitor.VisitKnowledgesPopulation( knowledgePopulationMap_.size() );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::Merge
// Created: LDC 2012-04-30
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::Merge( const DEC_BlackBoard_CanContainKnowledgePopulation& /*subGroup*/ )
{
}
