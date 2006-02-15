//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentKnowledge.inl $
// $Author: Ape $
// $Modtime: 28/05/04 15:43 $
// $Revision: 3 $
// $Workfile: AgentKnowledge.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
uint AgentKnowledge::GetID() const
{
    return nID_;    
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetPosition
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& AgentKnowledge::GetPosition() const
{
    return vPosition_;    
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetRealAgent
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
Agent& AgentKnowledge::GetRealAgent() const
{
    return realAgent_;    
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetOwner
/** @return 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
inline
KnowledgeGroup& AgentKnowledge::GetOwner() const
{
    return owner_;
}
