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
    CIT_KnowledgeAgentPerceptionMap itKnowledge = unitKnowledgePerceptionMap_.find( &associatedAgent );
    if ( itKnowledge != unitKnowledgePerceptionMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}

