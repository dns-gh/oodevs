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
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgePopulation )
BOOST_CLASS_TRACKING( DEC_BlackBoard_CanContainKnowledgePopulation, boost::serialization::track_never )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulation::DEC_BlackBoard_CanContainKnowledgePopulation( MIL_KnowledgeGroup* group )
    : pKnowledgeGroup_( group )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation constructor
// Created: LGY 2013-04-02
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulation::DEC_BlackBoard_CanContainKnowledgePopulation()
    : pKnowledgeGroup_( 0 )
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
    file >> const_cast< MIL_KnowledgeGroup*& >( pKnowledgeGroup_ );
    file >> knowledgePopulationMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgePopulation::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pKnowledgeGroup_;
    file << knowledgePopulationMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::CreateKnowledgePopulation
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& DEC_BlackBoard_CanContainKnowledgePopulation::CreateKnowledgePopulation( const MIL_KnowledgeGroup& group, MIL_Population& populationPerceived )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge( new DEC_Knowledge_Population( group, populationPerceived ) );
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
        if( it->second->GetID() == nID )
            return it->second;
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgesPopulation
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_BlackBoard_CanContainKnowledgePopulation::GetKnowledgePopulation( const MIL_Population& associatedPopulation ) const
{
    auto it = knowledgePopulationMap_.find( &associatedPopulation );
    if( it != knowledgePopulationMap_.end() )
        return it->second;
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
void DEC_BlackBoard_CanContainKnowledgePopulation::Merge( const DEC_BlackBoard_CanContainKnowledgePopulation& subGroup )
{
    for( auto itKnowledge = subGroup.knowledgePopulationMap_.begin(); itKnowledge != subGroup.knowledgePopulationMap_.end(); ++itKnowledge )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = GetKnowledgePopulation( *itKnowledge->first );
        boost::shared_ptr< DEC_Knowledge_Population > pSubKnowledge = itKnowledge->second;
        if( !pKnowledge.get() )
        {
            boost::shared_ptr< DEC_Knowledge_Population > copy( new DEC_Knowledge_Population( *pSubKnowledge, *pKnowledgeGroup_ ) );
            copy->CopyFrom( *pSubKnowledge );
            knowledgePopulationMap_.insert( std::make_pair( &copy->GetPopulationKnown(), copy ) );
        }
        else
            pKnowledge->Merge( *pSubKnowledge );
    }
}
