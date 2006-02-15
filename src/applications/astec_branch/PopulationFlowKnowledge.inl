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
// Name: PopulationFlowKnowledge::GetID
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
uint PopulationFlowKnowledge::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetKnowledgeGroup
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
const KnowledgeGroup& PopulationFlowKnowledge::GetKnowledgeGroup() const
{
    assert( pKnowledgeGroup_ );
    return *pKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetPopulationKnowledge
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
const PopulationKnowledge* PopulationFlowKnowledge::GetPopulationKnowledge() const
{
    return pPopulationKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetFlow
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
const PopulationFlow* PopulationFlowKnowledge::GetFlow() const
{
    return pFlow_;
}
    
// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetDirection
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float PopulationFlowKnowledge::GetDirection() const
{
    return rDirection_;
}
    
// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetSpeed
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float PopulationFlowKnowledge::GetSpeed() const
{
    return rSpeed_;
}
    
// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetNbrAliveHumans
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
uint PopulationFlowKnowledge::GetNbrAliveHumans() const
{
    return nNbrAliveHumans_;
}
    
// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetNbrDeadHumans
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
uint PopulationFlowKnowledge::GetNbrDeadHumans() const
{
    return nNbrDeadHumans_;
}
    
// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetAttitude
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
E_PopulationAttitude PopulationFlowKnowledge::GetAttitude() const
{
    return eAttitude_;
}
        
// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::IsPerceived
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
bool PopulationFlowKnowledge::IsPerceived() const
{
    return bIsPerceived_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetFlowParts
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
inline
const PopulationFlowKnowledge::T_FlowParts& PopulationFlowKnowledge::GetFlowParts() const
{
    return flowParts_;
}

