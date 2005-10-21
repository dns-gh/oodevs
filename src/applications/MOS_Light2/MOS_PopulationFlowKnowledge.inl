//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentKnowledge.inl $
// $Author: Ape $
// $Modtime: 28/05/04 15:43 $
// $Revision: 3 $
// $Workfile: MOS_AgentKnowledge.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetID
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationFlowKnowledge::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetGtia
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
const MOS_Gtia& MOS_PopulationFlowKnowledge::GetGtia() const
{
    assert( pGtia_ );
    return *pGtia_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetPopulationKnowledge
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
const MOS_PopulationKnowledge* MOS_PopulationFlowKnowledge::GetPopulationKnowledge() const
{
    return pPopulationKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetFlow
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
const MOS_PopulationFlow* MOS_PopulationFlowKnowledge::GetFlow() const
{
    return pFlow_;
}
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetPosition
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PopulationFlowKnowledge::GetPosition() const
{
    return position_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetDirection
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float MOS_PopulationFlowKnowledge::GetDirection() const
{
    return rDirection_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetSpeed
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float MOS_PopulationFlowKnowledge::GetSpeed() const
{
    return rSpeed_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetNbrAliveHumans
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationFlowKnowledge::GetNbrAliveHumans() const
{
    return nNbrAliveHumans_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetNbrDeadHumans
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationFlowKnowledge::GetNbrDeadHumans() const
{
    return nNbrDeadHumans_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetAttitude
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
E_PopulationAttitude MOS_PopulationFlowKnowledge::GetAttitude() const
{
    return eAttitude_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::GetRelevance
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float MOS_PopulationFlowKnowledge::GetRelevance() const
{
    return rRelevance_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::IsPerceived
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
bool MOS_PopulationFlowKnowledge::IsPerceived() const
{
    return bIsPerceived_;
}
