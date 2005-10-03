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
#include "MOS_App.h"


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
// Name: MOS_AgentManager::GetPopulationList
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
MOS_AgentManager::CT_PopulationMap& MOS_AgentManager::GetPopulationList()
{
	return populationMap_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindGtia
// Created: NLD 2004-03-18
//-----------------------------------------------------------------------------
inline
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
inline
void MOS_AgentManager::DeleteAllGtias()
{
    for( IT_TeamMap it = teamMap_.begin(); it != teamMap_.end(); ++it )
        it->second->DeleteAllGtias();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTeamFromIdx
// Created: NLD 2004-03-25
//-----------------------------------------------------------------------------
inline
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
inline
const MOS_AgentManager::T_TeamMap& MOS_AgentManager::GetTeams() const
{
    return teamMap_;    
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindTeam
// Created: NLD 2004-03-25
//-----------------------------------------------------------------------------
inline
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
inline
MOS_Team* MOS_AgentManager::FindTeam( const std::string& strName ) const
{
    for( MOS_AgentManager::CIT_TeamMap it = teamMap_.begin(); it != teamMap_.end(); ++it )
    {
        if( (*it).second->GetName() == strName )
            return (*it).second;
    }
    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::AddDirectConflict
/** @param  nConflictID 
    @param  origin 
    @param  target 
*/
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
inline
void MOS_AgentManager::AddDirectConflict( ASN1T_OID nConflictID, MOS_Agent& origin, MOS_Agent& target )
{
    T_Conflict conflict;
    conflict.pOrigin_ = &origin;
    conflict.pDirectFireTarget_ = &target;

    if( ! conflictMap_.insert( std::make_pair( nConflictID, conflict ) ).second )
        RUNTIME_ERROR;
    MOS_App::GetApp().NotifyAgentConflictStarted( origin );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::AddIndirectConflict
/** @param  nConflictID 
    @param  origin 
    @param  vTarget 
*/
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
inline
void MOS_AgentManager::AddIndirectConflict( ASN1T_OID nConflictID, MOS_Agent& origin, const MT_Vector2D& vTarget )
{
    T_Conflict conflict;
    conflict.pOrigin_ = &origin;
    conflict.pDirectFireTarget_ = 0;
    conflict.vIndirectFireTarget_ = vTarget;

    if( ! conflictMap_.insert( std::make_pair( nConflictID, conflict ) ).second )
        RUNTIME_ERROR;
    MOS_App::GetApp().NotifyAgentConflictStarted( origin );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::DeleteConflict
/** @param  nConflictID 
*/
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
inline
void MOS_AgentManager::DeleteConflict( ASN1T_OID nConflictID )
{
    IT_ConflictMap it = conflictMap_.find( nConflictID );
    if( it != conflictMap_.end() )
    {
        MOS_App::GetApp().NotifyAgentConflictEnded( *(*it).second.pOrigin_ );
        conflictMap_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentManager::FindConflictOrigin
/** @param  nConsignId 
    @return 
*/
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
inline
MOS_Agent* MOS_AgentManager::FindConflictOrigin( ASN1T_OID nConflictID )
{
    IT_ConflictMap it = conflictMap_.find( nConflictID );
    if( it != conflictMap_.end() )
    {
        return it->second.pOrigin_;
    }
    return 0;
}
