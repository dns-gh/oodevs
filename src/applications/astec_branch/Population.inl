// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

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
std::string Population::GetName() const
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
