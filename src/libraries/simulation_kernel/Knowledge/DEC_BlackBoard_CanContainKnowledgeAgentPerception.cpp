// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeAgentPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 19:15 $
// $Revision: 2 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeAgentPerception.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"

#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_AgentPerception.h"

#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgeAgentPerception, "DEC_BlackBoard_CanContainKnowledgeAgentPerception" )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgentPerception::DEC_BlackBoard_CanContainKnowledgeAgentPerception()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgentPerception::~DEC_BlackBoard_CanContainKnowledgeAgentPerception()
{
    while( !knowledgeAgentPerceptionMap_.empty() )
        DestroyKnowledgeAgentPerception( *knowledgeAgentPerceptionMap_.begin()->second );    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        void serialize( Archive& file, DEC_BlackBoard_CanContainKnowledgeAgentPerception::T_KnowledgeAgentPerceptionMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_BlackBoard_CanContainKnowledgeAgentPerception::T_KnowledgeAgentPerceptionMap& map, const uint )
        {
            file << map.size();
            for ( DEC_BlackBoard_CanContainKnowledgeAgentPerception::CIT_KnowledgeAgentPerceptionMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_BlackBoard_CanContainKnowledgeAgentPerception::T_KnowledgeAgentPerceptionMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_Agent_ABC* pAgent;
                
                file >> pAgent;
                file >> map[ pAgent ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> knowledgeAgentPerceptionMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << knowledgeAgentPerceptionMap_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::CreateKnowledgeAgentPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerception& DEC_BlackBoard_CanContainKnowledgeAgentPerception::CreateKnowledgeAgentPerception( const MIL_AgentPion& agentPerceiving, MIL_Agent_ABC& agentPerceived )
{
    DEC_Knowledge_AgentPerception* pKnowledge = new DEC_Knowledge_AgentPerception( agentPerceiving, agentPerceived );//$$ RAM   
    bool bOut = knowledgeAgentPerceptionMap_.insert( std::make_pair( &agentPerceived, pKnowledge ) ).second;
    assert( bOut );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::DestroyKnowledgeAgentPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::DestroyKnowledgeAgentPerception( DEC_Knowledge_AgentPerception& knowledge )
{
    int nOut = knowledgeAgentPerceptionMap_.erase( &knowledge.GetAgentPerceived() );
    assert( nOut == 1 );
    delete &knowledge;
}
