// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetID
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
inline
const MIL_AgentID MOS_Population::GetID() const
{
	return nPopulationID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetTeam
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
MOS_Team& MOS_Population::GetTeam() const
{
	return *pTeam_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetName
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
const std::string MOS_Population::GetName() const
{
	return strName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetModel
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
const MOS_AgentModel& MOS_Population::GetModel() const
{
    assert( pType_ );
    return pType_->GetModel();
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetType
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
const MOS_TypePopulation& MOS_Population::GetType() const
{
    assert( pType_ );
    return *pType_;
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

// -----------------------------------------------------------------------------
// Name: mos_population::FindConcentration
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
const MOS_PopulationConcentration* MOS_Population::FindConcentration( uint nID ) const
{
    CIT_ConcentrationMap it = concentrationMap_.find( nID );
    if( it == concentrationMap_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::FindFlow
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
const MOS_PopulationFlow* MOS_Population::FindFlow( uint nID ) const
{
    CIT_FlowMap it = flowMap_.find( nID );
    if( it == flowMap_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetConcentrations
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
inline
const MOS_Population::T_ConcentrationMap& MOS_Population::GetConcentrations() const
{
    return concentrationMap_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Population::GetFlows
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
inline
const MOS_Population::T_FlowMap& MOS_Population::GetFlows() const
{
    return flowMap_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::IsEmpty
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
inline
bool MOS_Population::IsEmpty() const
{
    return flowMap_.size() == 0 && concentrationMap_.size() == 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetAggregatedPos
// Created: HME 2006-01-24
// -----------------------------------------------------------------------------
inline
const MT_Vector2D MOS_Population::GetAggregatedPos() const
{
    return GetPos();
}

// -----------------------------------------------------------------------------
// Name: MOS_Population::GetConflictPos
// Created: HME 2006-01-24
// -----------------------------------------------------------------------------
inline
const MT_Vector2D  MOS_Population::GetConflictPos( const MT_Vector2D& origin ) const
{
    return origin + MT_Vector2D( 10, 10 );
}