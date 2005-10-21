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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Gtia.inl $
// $Author: Ape $
// $Modtime: 23/11/04 18:06 $
// $Revision: 8 $
// $Workfile: MOS_Gtia.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
uint MOS_Gtia::GetID() const
{
    return nID_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::GetAgentKnowledges
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
const MOS_Gtia::T_AgentKnowledgeMap& MOS_Gtia::GetAgentKnowledges() const
{
    return agentKnowledges_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::SetTeam
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
inline
void MOS_Gtia::SetTeam( MOS_Team& team )
{
    if( pTeam_ != 0 )
    {
        assert( pTeam_ == &team );
        return;
    }

    pTeam_ = &team;
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::SetType
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
inline
void MOS_Gtia::SetType( const std::string& strType )
{
    strType_ = strType;
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::GetTeam
// Created: APE 2004-05-27
// -----------------------------------------------------------------------------
inline
MOS_Team& MOS_Gtia::GetTeam() const
{
//$$$$ en attendant un refactoring des données coté SIM et de leur reception.
//    assert( pTeam_ != 0 );
    return *pTeam_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::GetPopulationKnowledges
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
inline
const MOS_Gtia::T_PopulationKnowledgeMap& MOS_Gtia::GetPopulationKnowledges() const
{
    return populationKnowledges_;
}
