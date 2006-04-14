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
    CIT_KnowledgeAgentMap it = realAgentMap_.find( &agentKnown );
    if( it == realAgentMap_.end() )
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
    return realAgentMap_.find( &agentKnown ) != realAgentMap_.end();
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgentFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent* DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgentFromID( uint nID ) const
{
    CIT_KnowledgeAgentIDMap itKnowledge = knowledgeAgentFromIDMap_.find( nID );
    return itKnowledge == knowledgeAgentFromIDMap_.end() ? 0 : itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetDetected
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
inline
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetDetected()
{
    UpdateQueriesCache();
    return detectedContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetFriends
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
inline
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetFriends()
{
    UpdateQueriesCache();
    return friendsContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetEnemies
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
inline
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetEnemies()
{
    UpdateQueriesCache();
    return enemiesContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetMilitias
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
inline
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetMilitias()
{
    UpdateQueriesCache();
    return militiasContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetTerrorists
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
inline
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetTerrorists()
{
    UpdateQueriesCache();
    return terroristsContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetRefugees
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
inline
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetRefugees()
{
    UpdateQueriesCache();
    return refugeesContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetSurrenderedAgents
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
inline
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetSurrenderedAgents()
{
    UpdateQueriesCache();
    return surrenderedAgentsContainer_;
}