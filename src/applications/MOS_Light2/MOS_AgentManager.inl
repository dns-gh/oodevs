//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentManager.inl $
// $Author: Ape $
// $Modtime: 23/11/04 18:03 $
// $Revision: 8 $
// $Workfile: MOS_AgentManager.inl $
//
//*****************************************************************************


#include "MOS_Team.h"


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::GetAgentList
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_AgentManager::CT_AgentMap& MOS_AgentManager::GetAgentList()
{
    return agentMap_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindGtia
// Created: NLD 2004-03-18
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_Gtia* MOS_AgentManager::FindGtia( uint nID )
{
    for( IT_TeamMap it = teamMap_.begin(); it != teamMap_.end(); ++it )
    {
        MOS_Gtia* pResult = it->second->FindGtia( nID );
        if( pResult != 0 )
            return pResult;
    }

    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::DeleteAllGtias
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_AgentManager::DeleteAllGtias()
{
    for( IT_TeamMap it = teamMap_.begin(); it != teamMap_.end(); ++it )
        it->second->DeleteAllGtias();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTeamFromIdx
// Created: NLD 2004-03-25
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_Team* MOS_AgentManager::FindTeamFromIdx( uint nIdx ) const
{
    for( CIT_TeamMap itTeam = teamMap_.begin(); itTeam != teamMap_.end(); ++itTeam )
    {
        if( itTeam->second->GetIdx() == nIdx )
            return itTeam->second;
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::GetTeams
// Created: NLD 2004-03-25
//-----------------------------------------------------------------------------
MOS_INLINE
const MOS_AgentManager::T_TeamMap& MOS_AgentManager::GetTeams() const
{
    return teamMap_;    
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTeam
// Created: NLD 2004-03-25
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_Team* MOS_AgentManager::FindTeam( uint nID ) const
{
    CIT_TeamMap itTeam = teamMap_.find( nID );
    return itTeam == teamMap_.end() ? 0 : itTeam->second;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTeam
/** @param  strName 
    @return 
    */
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Team* MOS_AgentManager::FindTeam( const std::string& strName ) const
{
    for( MOS_AgentManager::CIT_TeamMap it = teamMap_.begin(); it != teamMap_.end(); ++it )
    {
        if( (*it).second->GetName() == strName )
            return (*it).second;
    }
    return 0;
}

