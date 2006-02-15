// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/KnowledgeGroup.inl $
// $Author: Ape $
// $Modtime: 23/11/04 18:06 $
// $Revision: 8 $
// $Workfile: KnowledgeGroup.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
uint KnowledgeGroup::GetID() const
{
    return nID_;
}


// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetAgentKnowledges
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
const KnowledgeGroup::T_AgentKnowledgeMap& KnowledgeGroup::GetAgentKnowledges() const
{
    return agentKnowledges_;    
}


// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SetTeam
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
inline
void KnowledgeGroup::SetTeam( Team& team )
{
    if( pTeam_ != 0 )
    {
        assert( pTeam_ == &team );
        return;
    }

    pTeam_ = &team;
}


// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SetType
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
inline
void KnowledgeGroup::SetType( const std::string& strType )
{
    strType_ = strType;
}


// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetTeam
// Created: APE 2004-05-27
// -----------------------------------------------------------------------------
inline
Team& KnowledgeGroup::GetTeam() const
{
//$$$$ en attendant un refactoring des données coté SIM et de leur reception.
//    assert( pTeam_ != 0 );
    return *pTeam_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetPopulationKnowledges
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
inline
const KnowledgeGroup::T_PopulationKnowledgeMap& KnowledgeGroup::GetPopulationKnowledges() const
{
    return populationKnowledges_;
}
