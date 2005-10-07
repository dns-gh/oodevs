// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetPopulationID
/** @return 
*/
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
MIL_AgentID MOS_Population::GetPopulationID()
{
	return nPopulationID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetTeam
/** @return 
*/
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
MOS_Team& MOS_Population::GetTeam() const
{
	return *pTeam_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetName
/** @return 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
const std::string MOS_Population::GetName() const
{
	return sName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetID
/** @return 
*/
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
inline
const uint MOS_Population::GetID()
{
	return GetPopulationID();
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetModel
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
const MOS_AgentModel& MOS_Population::GetModel() const
{
    assert( pTypePopulation_ );
    return pTypePopulation_->GetModel();
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::FindAgentKnowledge
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
MOS_AgentKnowledge* MOS_Population::FindAgentKnowledge( uint nId )
{
    IT_AgentKnowledgeMap it = agentKnowledges_.find( nId );    
    if( it != agentKnowledges_.end() )
        return it->second;
    return 0;
}
