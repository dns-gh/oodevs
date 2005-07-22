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

#include "MOS_AgentKnowledge.h"

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_Gtia::GetID() const
{
    return nID_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::GetAgentKnowledges
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Gtia::T_AgentKnowledgeMap& MOS_Gtia::GetAgentKnowledges() const
{
    return agentKnowledges_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::SetTeam
/** @param  team 
*/
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
MOS_INLINE
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
MOS_INLINE
void MOS_Gtia::SetType( const std::string& strType )
{
    strType_ = strType;
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::GetTeam
/** @return 
*/
// Created: APE 2004-05-27
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Team& MOS_Gtia::GetTeam() const
{
//$$$$ en attendant un refactoring des données coté SIM et de leur reception.
//    assert( pTeam_ != 0 );
    return *pTeam_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Gtia::FindAgentKnowledge
/** @param  nId 
    @return 
*/
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_AgentKnowledge* MOS_Gtia::FindAgentKnowledge( int nId )
{
    IT_AgentKnowledgeMap it = agentKnowledges_.find( nId );    
    if( it != agentKnowledges_.end() )
        return it->second;
    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_Gtia::FindKnowledgeOnAgent
/** @param  agent 
    @return 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_AgentKnowledge* MOS_Gtia::FindKnowledgeOnAgent( const MOS_Agent& agent )
{
    for( IT_AgentKnowledgeMap it = agentKnowledges_.begin(); it != agentKnowledges_.end(); ++it )
        if( &(it->second->GetRealAgent()) == &agent )
            return it->second;

    return 0;
}
