// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeAgentPerception.inl $
// $Author: Jvt $
// $Modtime: 6/04/05 17:32 $
// $Revision: 2 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeAgentPerception.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::GetKnowledgesAgentPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_AgentPerception* DEC_BlackBoard_CanContainKnowledgeAgentPerception::GetKnowledgeAgentPerception( const MIL_Agent_ABC& associatedAgent ) const
{
    CIT_KnowledgeAgentPerceptionMap itKnowledge = knowledgeAgentPerceptionMap_.find( &associatedAgent );
    if ( itKnowledge != knowledgeAgentPerceptionMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::GetAttackersKnowledgeAgentPerception
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
inline
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::GetAttackersKnowledgeAgentPerception( T_KnowledgeAgentPerceptionVector& container ) const
{
    std::copy( attackerSet_.begin(), attackerSet_.end(), std::back_inserter( container ) );
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::NotifyKnowledgeAgentPerceptionIsAttacker
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
inline
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::NotifyKnowledgeAgentPerceptionIsAttacker( DEC_Knowledge_AgentPerception& knowledge )
{
    attackerSet_.insert( &knowledge );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgentPerception::NotifyKnowledgesAgentPerceptionNotAttackers
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
inline
void DEC_BlackBoard_CanContainKnowledgeAgentPerception::NotifyKnowledgesAgentPerceptionNotAttackers()
{
    attackerSet_.clear();
}