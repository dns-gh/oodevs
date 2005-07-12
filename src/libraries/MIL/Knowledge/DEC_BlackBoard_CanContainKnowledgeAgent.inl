// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.inl $
// $Author: Nld $
// $Modtime: 20/04/05 19:04 $
// $Revision: 2 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeAgent.inl $
//
// *****************************************************************************

// =============================================================================
// QUERIES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgent
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent* DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgent( const MIL_Agent_ABC& agentKnown ) const
{
    CIT_KnowledgeAgentMap it = knowledgeAgentMap_.find( &agentKnown );
    if( it == knowledgeAgentMap_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::HasKnowledgeAgent
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
inline
bool DEC_BlackBoard_CanContainKnowledgeAgent::HasKnowledgeAgent( const MIL_Agent_ABC& agentKnown ) const
{
    return knowledgeAgentMap_.find( &agentKnown ) != knowledgeAgentMap_.end();
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgentFromMosID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent* DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgentFromMosID( uint nID ) const
{
    CIT_KnowledgeAgentIDMap itKnowledge = knowledgeAgentFromMosIDMap_.find( nID );
    return itKnowledge == knowledgeAgentFromMosIDMap_.end() ? 0 : itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgentFromDiaID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent* DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgentFromDiaID( uint nID ) const
{
    CIT_KnowledgeAgentIDMap itKnowledge = knowledgeAgentFromDiaIDMap_.find( nID );
    return itKnowledge == knowledgeAgentFromDiaIDMap_.end() ? 0 : itKnowledge->second;
}
