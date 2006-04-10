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
#include "MIL_PopulationType.h"
#include "Entities/Populations/Actions/PHY_FireResults_Population.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Effects/MIL_Effect_PopulationFire.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "MIL_AgentServer.h"

MT_Random MIL_PopulationElement_ABC::randomGenerator_;

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
// Name: MIL_PopulationElement_ABC::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationElement_ABC::GetDangerosity( const MIL_AgentPion& target ) const
{
    assert( pAttitude_   );
    assert( pPopulation_ );

    if( target.GetRole< PHY_RoleInterface_Population >().IsInvulnerable() )
        return 0.;

    const PHY_ComposantePion* pTargetComposante = target.GetRole< PHY_RolePion_Composantes >().GetMajorComposante();
    if( !pTargetComposante )
        return 0.;

    const MT_Float           rPH           = pPopulation_->GetType().GetPH( *pAttitude_, rDensity_ );
    const PHY_AttritionData& attritionData = pPopulation_->GetType().GetAttritionData( *pAttitude_, pTargetComposante->GetType().GetProtection() );

    return rPH * attritionData.GetScore();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::FireOnPion
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::FireOnPion( MT_Float rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult )
{
    assert( pAttitude_ );
    assert( pPopulation_ );
    if( target.GetRole< PHY_RoleInterface_Population >().IsInvulnerable() )
        return;

    PHY_RoleInterface_Composantes::T_ComposanteVector compTargets;
    target.GetRole< PHY_RoleInterface_Composantes >().GetComposantesAbleToBeFired( compTargets );
    if( compTargets.empty() )
        return;

    target.NotifyAttackedBy( *pPopulation_ );

    const MT_Float rPH = GetPopulation().GetType().GetPH( *pAttitude_, rDensity_ );
    if ( !( randomGenerator_.rand_oi() <= rPH * rIntensity ) ) 
        return;

    MIL_Effect_PopulationFire* pEffect = new MIL_Effect_PopulationFire( GetPopulation().GetType(), GetAttitude(), target, *compTargets.front(), fireResult );
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( *pEffect );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::FireOnPions
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::FireOnPions( MT_Float rIntensity, PHY_FireResults_Population& fireResult )
{
    assert( pAttitude_ );
    for( CIT_AgentVector it = collidingAgents_.begin(); it != collidingAgents_.end(); ++it )
    {
        MIL_Agent_ABC& target = **it;
        if( target.GetArmy().IsAnEnemy( GetPopulation().GetArmy() ) != eTristate_True )
            continue;
        FireOnPion( rIntensity, target, fireResult );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ApplyFire
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ApplyFire( uint nNbrAmmoFired, PHY_FireResults_ABC& fireResult )
{
    assert( pPopulation_ );

    MT_Float rTmp = std::min( rNbrAliveHumans_, (MT_Float)nNbrAmmoFired );
    if( rTmp == 0. )
        return;

    rNbrDeadHumans_  += rTmp;
    rNbrAliveHumans_ -= rTmp;
    bHumansUpdated_ = true;

    fireResult.GetDamages( *pPopulation_ ).NotifyHumansKilled( (uint)rTmp );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ApplyIndirectFire
// Created: SBO 2006-01-19
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ApplyIndirectFire( const MT_Circle& attritionCircle, PHY_FireResults_ABC& fireResult )
{
    assert( pPopulation_ );
    // $$$$ SBO 2006-04-07: 2% kill, at least one kill
    MT_Float rTmp = ceil( 0.02f * std::min( rNbrAliveHumans_, rDensity_ * GetLocation().GetIntersectionAreaWithCircle( attritionCircle ) ) );

    rNbrDeadHumans_  += rTmp;
    rNbrAliveHumans_ -= rTmp;
    bHumansUpdated_   = true;

    fireResult.GetDamages( *pPopulation_ ).NotifyHumansKilled( (uint)rTmp );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::UpdateCollisions
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::UpdateCollisions()
{
    TER_Agent_ABC::T_AgentPtrVector agents;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( GetLocation(), agents, 100. ); //$$$ TEST
    collidingAgents_.clear(); collidingAgents_.reserve( agents.size() );
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

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetSecuringPoint
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
MT_Vector2D MIL_PopulationElement_ABC::GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const
{
    MT_Vector2D result;
    GetLocation().ComputeNearestPoint( securingAgent.GetRole< PHY_RolePion_Location >().GetPosition(), result );
    return result;
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
