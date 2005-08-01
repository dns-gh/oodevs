// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.cpp $
// $Author: Nld $
// $Modtime: 20/04/05 19:05 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeAgent.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"

#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_Agent.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgeAgent, "DEC_BlackBoard_CanContainKnowledgeAgent" )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgent::DEC_BlackBoard_CanContainKnowledgeAgent()
{
	
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgent::~DEC_BlackBoard_CanContainKnowledgeAgent()
{
    while( !knowledgeAgentFromDiaIDMap_.empty() )
        DestroyKnowledgeAgent( *knowledgeAgentFromDiaIDMap_.begin()->second );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& map, const uint )
        {
            file << map.size();
            for ( DEC_BlackBoard_CanContainKnowledgeAgent::CIT_KnowledgeAgentMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_Agent_ABC*       pAgent;
                DEC_Knowledge_Agent* pKnowledge;
                
                file >> pAgent
                     >> pKnowledge;
                     
                map.insert( std::make_pair( pAgent, pKnowledge ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::load
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> knowledgeAgentMap_
         >> knowledgeAgentFromMosIDMap_
         >> knowledgeAgentFromDiaIDMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::save
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << knowledgeAgentMap_
         << knowledgeAgentFromMosIDMap_
         << knowledgeAgentFromDiaIDMap_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent& DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& agentKnown )
{
    DEC_Knowledge_Agent& knowledge = agentKnown.CreateKnowledge( knowledgeGroup );
    knowledgeAgentMap_.insert( std::make_pair( &agentKnown, &knowledge ) );

    bool bOut = knowledgeAgentFromMosIDMap_.insert( std::make_pair( knowledge.GetMosID(), &knowledge ) ).second;
    assert( bOut );
    bOut = knowledgeAgentFromDiaIDMap_.insert( std::make_pair( knowledge.GetDiaID(), &knowledge ) ).second;
    assert( bOut );

    return knowledge;
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::DestroyKnowledgeAgent
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::DestroyKnowledgeAgent( DEC_Knowledge_Agent& knowledge )
{
    int nOut = knowledgeAgentMap_.erase( &knowledge.GetAgentKnown() );
    assert( nOut >= 1 );
    nOut = knowledgeAgentFromMosIDMap_.erase( knowledge.GetMosID() );
    assert( nOut == 1 );
    nOut = knowledgeAgentFromDiaIDMap_.erase( knowledge.GetDiaID() );
    assert( nOut == 1 );
    delete &knowledge;
}

