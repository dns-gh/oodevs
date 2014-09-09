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
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeAgentPerception )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgentPerception::DEC_BlackBoard_CanContainKnowledgeAgentPerception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgentPerception::~DEC_BlackBoard_CanContainKnowledgeAgentPerception()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> perceptions_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << perceptions_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::CreateKnowledgeAgentPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerception& DEC_BlackBoard_CanContainKnowledgeAgentPerception::CreateKnowledgeAgentPerception( const MIL_Agent_ABC& agentPerceiving, const MIL_Agent_ABC& agentPerceived )
{
    DEC_Knowledge_AgentPerception* pKnowledge = new DEC_Knowledge_AgentPerception( agentPerceiving, agentPerceived );//$$ RAM
    if( ! perceptions_.insert( std::make_pair( &agentPerceived, pKnowledge ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::DestroyKnowledgeAgentPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::DestroyKnowledgeAgentPerception( DEC_Knowledge_AgentPerception& knowledge )
{
    if( perceptions_.erase( &knowledge.GetAgentPerceived() ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::CleanDeletedAgentKnowledges()
{
    for( auto it = perceptions_.begin(); it != perceptions_.end(); )
    {
        if( it->first->IsMarkedForDestruction() )
            it = perceptions_.erase( it );
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::Clear
// Created: LDC 2014-09-08
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::Clear()
{
    while( !perceptions_.empty() )
        DestroyKnowledgeAgentPerception( *perceptions_.begin()->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::GetKnowledgesAgentPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerception* DEC_BlackBoard_CanContainKnowledgeAgentPerception::GetKnowledgeAgentPerception( const MIL_Agent_ABC& associatedAgent ) const
{
    auto it = perceptions_.find( &associatedAgent );
    if( it != perceptions_.end() )
        return it->second;
    else
        return 0;
}
