// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Population::GetID
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
inline
const MIL_AgentID Population::GetID() const
{
	return nPopulationID_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetTeam
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
Team& Population::GetTeam() const
{
	return *pTeam_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetName
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
const std::string Population::GetName() const
{
	return strName_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetModel
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
const AgentModel& Population::GetModel() const
{
    assert( pType_ );
    return pType_->GetModel();
}

// -----------------------------------------------------------------------------
// Name: Population::GetType
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
const TypePopulation& Population::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: Population::FindAgentKnowledge
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
AgentKnowledge* Population::FindAgentKnowledge( uint nId )
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
const PopulationConcentration* Population::FindConcentration( uint nID ) const
{
    CIT_ConcentrationMap it = concentrationMap_.find( nID );
    if( it == concentrationMap_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: Population::FindFlow
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
const PopulationFlow* Population::FindFlow( uint nID ) const
{
    CIT_FlowMap it = flowMap_.find( nID );
    if( it == flowMap_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: Population::GetConcentrations
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
inline
const Population::T_ConcentrationMap& Population::GetConcentrations() const
{
    return concentrationMap_;
}
    
// -----------------------------------------------------------------------------
// Name: Population::GetFlows
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
inline
const Population::T_FlowMap& Population::GetFlows() const
{
    return flowMap_;
}

// -----------------------------------------------------------------------------
// Name: Population::IsEmpty
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
inline
bool Population::IsEmpty() const
{
    return flowMap_.size() == 0 && concentrationMap_.size() == 0;
}


// -----------------------------------------------------------------------------
// Name: Population::GetAggregatedPos
// Created: HME 2006-01-24
// -----------------------------------------------------------------------------
inline
const MT_Vector2D Population::GetAggregatedPos() const
{
    return GetPos();
}

// -----------------------------------------------------------------------------
// Name: Population::GetConflictPos
// Created: HME 2006-01-24
// -----------------------------------------------------------------------------
inline
const MT_Vector2D  Population::GetConflictPos( const MT_Vector2D& origin ) const
{
    return origin + MT_Vector2D( 10, 10 );
}
