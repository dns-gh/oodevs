// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "MIL_PopulationElement_ABC.h"

#include "MIL_PopulationAttitude.h"
#include "MIL_Population.h"
#include "Entities/Populations/Actions/PHY_PopulationFireResults.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Effects/MIL_Effect_PopulationFire.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/MIL_EntityManager.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationElement_ABC::MIL_PopulationElement_ABC( MIL_Population& population, uint nID )
    : pPopulation_     ( &population )
    , nID_             ( nID )
    , rNbrAliveHumans_ ( 0. )
    , rNbrDeadHumans_  ( 0. )
    , rDensity_        ( 0. )
    , pAttitude_       ( &population.GetDefaultAttitude() )
    , bAttitudeUpdated_( true )
    , bHumansUpdated_  ( true )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_PopulationElement_ABC::MIL_PopulationElement_ABC()
    : pPopulation_     ( 0 )
    , nID_             ( 0 )
    , rNbrAliveHumans_ ( 0 )
    , rNbrDeadHumans_  ( 0 )
    , rDensity_        ( 0. )
    , pAttitude_       ( 0 )
    , bAttitudeUpdated_( true )
    , bHumansUpdated_  ( true )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationElement_ABC::~MIL_PopulationElement_ABC()
{
} 

// =============================================================================
// ACTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::FireOnPions
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::FireOnPions( PHY_PopulationFireResults& fireResult )
{
    for( CIT_AgentVector it = collidingAgents_.begin(); it != collidingAgents_.end(); ++it )
    {
        MIL_Agent_ABC& target = **it;

        PHY_RoleInterface_Composantes::T_ComposanteVector compTargets;
        target.GetRole< PHY_RoleInterface_Composantes >().GetComposantesAbleToBeFired( compTargets );
        if( compTargets.empty() )
            continue;

        MIL_Effect_PopulationFire* pEffect = new MIL_Effect_PopulationFire( GetPopulation().GetType(), GetAttitude(), target, *compTargets.front(), fireResult );
        MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( *pEffect );
    }    
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::UpdateCollisions
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::UpdateCollisions()
{
    collidingAgents_.clear();

    TER_Agent_ABC::T_AgentPtrVector agents;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( GetLocation(), agents, 100. ); //$$$ TEST
    for( TER_Agent_ABC::CIT_AgentPtrVector it = agents.begin(); it != agents.end(); ++it )
    {
        MIL_Agent_ABC& agent = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
        collidingAgents_.push_back( &agent );
        
        NotifyCollision( agent );
    }
}

// =============================================================================
// PION EFFECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetPionMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationElement_ABC::GetPionMaxSpeed( const PHY_Volume& pionVolume ) const
{
    assert( pAttitude_ );
    return pPopulation_->GetPionMaxSpeed( *pAttitude_, rDensity_, pionVolume );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetPionReloadingTimeFactor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationElement_ABC::GetPionReloadingTimeFactor() const
{
    return pPopulation_->GetPionReloadingTimeFactor( rDensity_ );
}

// =============================================================================
// CHECKPOINT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::load
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> pPopulation_
         >> const_cast< uint& >( nID_ )
         >> rNbrAliveHumans_
         >> rNbrDeadHumans_
         >> rDensity_
         >> collidingAgents_;

    uint nAttitudeID;
    file >> nAttitudeID;
    pAttitude_ = MIL_PopulationAttitude::Find( nAttitudeID );
    assert( pAttitude_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << pPopulation_
         << nID_
         << rNbrAliveHumans_
         << rNbrDeadHumans_
         << rDensity_
         << collidingAgents_
         << pAttitude_->GetID();
}
