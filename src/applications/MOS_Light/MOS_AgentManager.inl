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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentManager.inl $
// $Author: Nld $
// $Modtime: 14/02/05 15:56 $
// $Revision: 8 $
// $Workfile: MOS_AgentManager.inl $
//
//*****************************************************************************

#include "MOS_Team.h"

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::GetAgentList
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
MOS_AgentManager::CT_AgentMap& MOS_AgentManager::GetAgentList()
{
    return agentMap_;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindGtia
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
MOS_Gtia* MOS_AgentManager::FindGtia( uint nID )
{
    IT_GtiaMap itGtia = gtiaMap_.find( nID );    
    if( itGtia != gtiaMap_.end() )
        return itGtia->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::GetTeams
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
const MOS_AgentManager::T_TeamIDVector& MOS_AgentManager::GetTeams() const
{
    return teams_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTeam
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
MOS_Team* MOS_AgentManager::FindTeam( uint nID ) const
{
    CIT_TeamMap itTeam = teamMap_.find( nID );
    return itTeam == teamMap_.end() ? 0 : itTeam->second;
}

