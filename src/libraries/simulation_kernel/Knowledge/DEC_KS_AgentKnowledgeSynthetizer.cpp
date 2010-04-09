// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_AgentKnowledgeSynthetizer.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 11:01 $
// $Revision: 7 $
// $Workfile: DEC_KS_AgentKnowledgeSynthetizer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_AgentKnowledgeSynthetizer.h"

#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_Agent.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/MIL_AgentServer.h" // LTO
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h" // LTO
#include <boost/bind.hpp>
#include <boost/function.hpp>
 
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_AgentKnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AgentKnowledgeSynthetizer::DEC_KS_AgentKnowledgeSynthetizer( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_           ( &blackBoard )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_AgentKnowledgeSynthetizer::DEC_KS_AgentKnowledgeSynthetizer()
    : DEC_KnowledgeSource_ABC()
    , pBlackBoard_           ( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AgentKnowledgeSynthetizer::~DEC_KS_AgentKnowledgeSynthetizer()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::Prepare()
{
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( & DEC_Knowledge_Agent::Prepare, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Talk
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::Talk( int currentTimeStep )
{
    assert( pBlackBoard_ );

    pBlackBoard_->ApplyOnKnowledgesAgentPerception( currentTimeStep ); 

    if( ! pBlackBoard_->GetKnowledgeGroup().IsJammedKnowledgeGroup() )
    {
        // Extrapolation
        pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( & DEC_Knowledge_Agent::Extrapolate, _1 ) );
        // Relevance
        pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( &DEC_Knowledge_Agent::UpdateRelevance, _1, currentTimeStep ) );
    }

}

// =============================================================================
// CLEAN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::CleanKnowledgeAgent
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::CleanKnowledgeAgent( DEC_Knowledge_Agent& knowledge )
{
    assert( pBlackBoard_ );

    if( knowledge.Clean() )
        pBlackBoard_->GetKnowledgeAgentContainer().DestroyKnowledgeAgent( knowledge ); // The knowledge will be deleted
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::Clean()
{
    assert( pBlackBoard_ );

    class_mem_fun_void_t< DEC_KS_AgentKnowledgeSynthetizer, DEC_Knowledge_Agent > methodAgent( & DEC_KS_AgentKnowledgeSynthetizer::CleanKnowledgeAgent, *this );
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( methodAgent );
}
