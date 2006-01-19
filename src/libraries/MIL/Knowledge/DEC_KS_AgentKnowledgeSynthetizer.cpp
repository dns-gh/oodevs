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

#include "MIL_pch.h"
#include "DEC_KS_AgentKnowledgeSynthetizer.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_Agent.h"

#include "MIL_KnowledgeGroup.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
 
// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AgentKnowledgeSynthetizer::DEC_KS_AgentKnowledgeSynthetizer( DEC_KnowledgeBlackBoard& blackBoard, const MIL_KnowledgeGroup& knowledgeGroup )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pKnowledgeGroup_       ( &knowledgeGroup )
{
    assert( pBlackBoard_ );
    pBlackBoard_->AddToScheduler( *this );       
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AgentKnowledgeSynthetizer::~DEC_KS_AgentKnowledgeSynthetizer()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );
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
    pBlackBoard_->ApplyOnKnowledgesAgent( std::mem_fun_ref( & DEC_Knowledge_Agent::Prepare ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::GetKnowledgeToUpdate
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent& DEC_KS_AgentKnowledgeSynthetizer::GetKnowledgeToUpdate( MIL_Agent_ABC& agentKnown ) const
{
    assert( pBlackBoard_ );
    DEC_Knowledge_Agent* pKnowledge = pBlackBoard_->GetKnowledgeAgent( agentKnown );
    if( pKnowledge )
        return *pKnowledge;
    
    assert( pKnowledgeGroup_ );
    return pBlackBoard_->CreateKnowledgeAgent( *pKnowledgeGroup_, agentKnown );
}
  
// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromAgentPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
void DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromAgentPerception( const DEC_Knowledge_AgentPerception& perception )
{
    if( perception.IsAvailable() )
        GetKnowledgeToUpdate( perception.GetAgentPerceived() ).Update( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Talk
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::Talk()
{
    assert( pKnowledgeGroup_ );
    assert( pBlackBoard_ );

    class_mem_fun_void_const_t< DEC_KS_AgentKnowledgeSynthetizer, DEC_Knowledge_AgentPerception> method( DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromAgentPerception, *this );

    // Synthèse de la perception des subordonnés
    // Ajout automatique de la connaissance de chaque subordonné    
    const MIL_KnowledgeGroup::T_AutomateVector& automates = pKnowledgeGroup_->GetAutomates();
    for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
    {
        const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        {
            MIL_AgentPion& pion = **itPion;
            pion.GetKnowledge().ApplyOnKnowledgesAgentPerception( method );
        }
    }
    
    // Extrapolation
    pBlackBoard_->ApplyOnKnowledgesAgent( std::mem_fun_ref( & DEC_Knowledge_Agent::Extrapolate ) );

    // Relevance
    pBlackBoard_->ApplyOnKnowledgesAgent( std::mem_fun_ref( & DEC_Knowledge_Agent::UpdateRelevance ) );
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
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->DestroyKnowledgeAgent( knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::Clean()
{
    assert( pBlackBoard_ );

    class_mem_fun_void_t< DEC_KS_AgentKnowledgeSynthetizer, DEC_Knowledge_Agent > methodAgent( DEC_KS_AgentKnowledgeSynthetizer::CleanKnowledgeAgent, *this );
    pBlackBoard_->ApplyOnKnowledgesAgent( methodAgent );
}