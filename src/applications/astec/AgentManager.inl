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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentManager.inl $
// $Author: Ape $
// $Modtime: 23/11/04 18:03 $
// $Revision: 8 $
// $Workfile: AgentManager.inl $
//
//*****************************************************************************


#include "Team.h"
#include "App.h"


//-----------------------------------------------------------------------------
// Name: AgentManager::GetAgentList
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
AgentManager::CT_AgentMap& AgentManager::GetAgentList()
{
    return agentMap_;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::GetPopulationList
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
inline
AgentManager::CT_PopulationMap& AgentManager::GetPopulationList()
{
	return populationMap_;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::GetAllAgentList
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
AgentManager::CT_AllAgentMap& AgentManager::GetAllAgentList()
{
    return allMap_;
}

//-----------------------------------------------------------------------------
// Name: AgentManager::FindGtia
// Created: NLD 2004-03-18
//-----------------------------------------------------------------------------
inline
Gtia* AgentManager::FindGtia( uint nID )
{
    for( IT_TeamMap it = teamMap_.begin(); it != teamMap_.end(); ++it )
    {
        Gtia* pResult = it->second->FindGtia( nID );
        if( pResult )
            return pResult;
    }

    return 0;
}


// -----------------------------------------------------------------------------
// Name: AgentManager::DeleteAllGtias
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
inline
void AgentManager::DeleteAllGtias()
{
    for( IT_TeamMap it = teamMap_.begin(); it != teamMap_.end(); ++it )
        it->second->DeleteAllGtias();
}


//-----------------------------------------------------------------------------
// Name: AgentManager::FindTeamFromIdx
// Created: NLD 2004-03-25
//-----------------------------------------------------------------------------
inline
Team* AgentManager::FindTeamFromIdx( uint nIdx ) const
{
    for( CIT_TeamMap itTeam = teamMap_.begin(); itTeam != teamMap_.end(); ++itTeam )
        if( itTeam->second->GetIdx() == nIdx )
            return itTeam->second;
    return 0;
}


//-----------------------------------------------------------------------------
// Name: AgentManager::GetTeams
// Created: NLD 2004-03-25
//-----------------------------------------------------------------------------
inline
const AgentManager::T_TeamMap& AgentManager::GetTeams() const
{
    return teamMap_;    
}


//-----------------------------------------------------------------------------
// Name: AgentManager::FindTeam
// Created: NLD 2004-03-25
//-----------------------------------------------------------------------------
inline
Team* AgentManager::FindTeam( uint nID ) const
{
    CIT_TeamMap itTeam = teamMap_.find( nID );
    return itTeam == teamMap_.end() ? 0 : itTeam->second;
}


// -----------------------------------------------------------------------------
// Name: AgentManager::FindTeam
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
inline
Team* AgentManager::FindTeam( const std::string& strName ) const
{
    for( AgentManager::CIT_TeamMap it = teamMap_.begin(); it != teamMap_.end(); ++it )
        if( (*it).second->GetName() == strName )
            return (*it).second;
    return 0;
}


// -----------------------------------------------------------------------------
// Name: AgentManager::AddDirectConflict
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
inline
void AgentManager::AddDirectConflict( ASN1T_OID nConflictID, Agent_ABC& origin, Agent_ABC& target )
{
    T_Conflict conflict;
    conflict.pOrigin_ = &origin;
    conflict.pDirectFireTarget_ = &target;

    if( ! conflictMap_.insert( std::make_pair( nConflictID, conflict ) ).second )
        RUNTIME_ERROR;
    App::GetApp().NotifyConflictStarted( origin );
}

// -----------------------------------------------------------------------------
// Name: AgentManager::AddIndirectConflict
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
inline
void AgentManager::AddIndirectConflict( ASN1T_OID nConflictID, Agent_ABC& origin, const MT_Vector2D& vTarget )
{
    T_Conflict conflict;
    conflict.pOrigin_ = &origin;
    conflict.pDirectFireTarget_ = 0;
    conflict.vIndirectFireTarget_ = vTarget;

    if( ! conflictMap_.insert( std::make_pair( nConflictID, conflict ) ).second )
        RUNTIME_ERROR;
    App::GetApp().NotifyConflictStarted( origin );
}


// -----------------------------------------------------------------------------
// Name: AgentManager::DeleteConflict
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
inline
void AgentManager::DeleteConflict( ASN1T_OID nConflictID )
{
    IT_ConflictMap it = conflictMap_.find( nConflictID );
    if( it != conflictMap_.end() )
    {
        App::GetApp().NotifyConflictEnded( *(*it).second.pOrigin_ );
        conflictMap_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentManager::FindConflictOrigin
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
inline
Agent_ABC* AgentManager::FindConflictOrigin( ASN1T_OID nConflictID )
{
    IT_ConflictMap it = conflictMap_.find( nConflictID );
    if( it != conflictMap_.end() )
        return it->second.pOrigin_;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::GetTypePopulations
// Created: HME 2005-10-20
// -----------------------------------------------------------------------------
inline
const AgentManager::T_TypePopulationMap* AgentManager::GetTypePopulations() const
{
    return &typesPopulation_;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::GetMaintenanceConsigns
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
inline
const AgentManager::T_MaintenanceConsigns& AgentManager::GetMaintenanceConsigns() const
{
    return maintenanceConsigns_;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::GetMedicalConsigns
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
inline
const AgentManager::T_MedicalConsigns& AgentManager::GetMedicalConsigns() const
{
    return medicalConsigns_;
}

// -----------------------------------------------------------------------------
// Name: AgentManager::GetSupplyConsigns
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
inline
const AgentManager::T_SupplyConsigns& AgentManager::GetSupplyConsigns() const
{
    return supplyConsigns_;
}