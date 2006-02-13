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

// -----------------------------------------------------------------------------
// Name: AgentManager::AddDirectConflict
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
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
const AgentManager::T_TypePopulationMap* AgentManager::GetTypePopulations() const
{
    return &typesPopulation_;
}
