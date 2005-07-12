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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Gtia.inl $
// $Author: Nld $
// $Modtime: 25/10/04 16:20 $
// $Revision: 2 $
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
// Name: MOS_Gtia::GetTeam
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
inline
const MOS_Team& MOS_Gtia::GetTeam() const
{
    assert( pArmy_ );
    return *pArmy_;
}
