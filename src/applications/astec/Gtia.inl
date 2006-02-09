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
// $Archive: /MVW_v10/Build/SDK/Light2/src/Gtia.inl $
// $Author: Ape $
// $Modtime: 23/11/04 18:06 $
// $Revision: 8 $
// $Workfile: Gtia.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Gtia::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
uint Gtia::GetID() const
{
    return nID_;
}


// -----------------------------------------------------------------------------
// Name: Gtia::GetAgentKnowledges
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
const Gtia::T_AgentKnowledgeMap& Gtia::GetAgentKnowledges() const
{
    return agentKnowledges_;    
}


// -----------------------------------------------------------------------------
// Name: Gtia::SetTeam
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
inline
void Gtia::SetTeam( Team& team )
{
    if( pTeam_ != 0 )
    {
        assert( pTeam_ == &team );
        return;
    }

    pTeam_ = &team;
}


// -----------------------------------------------------------------------------
// Name: Gtia::SetType
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
inline
void Gtia::SetType( const std::string& strType )
{
    strType_ = strType;
}


// -----------------------------------------------------------------------------
// Name: Gtia::GetTeam
// Created: APE 2004-05-27
// -----------------------------------------------------------------------------
inline
Team& Gtia::GetTeam() const
{
//$$$$ en attendant un refactoring des données coté SIM et de leur reception.
//    assert( pTeam_ != 0 );
    return *pTeam_;
}

// -----------------------------------------------------------------------------
// Name: Gtia::GetPopulationKnowledges
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
inline
const Gtia::T_PopulationKnowledgeMap& Gtia::GetPopulationKnowledges() const
{
    return populationKnowledges_;
}
