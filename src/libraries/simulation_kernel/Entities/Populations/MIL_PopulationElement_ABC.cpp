// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationElement_ABC.h"
#include "MIL_PopulationAttitude.h"
#include "MIL_Population.h"
#include "MIL_PopulationType.h"
#include "MIL_IntoxicationEffect.h"
#include "MIL_ContaminationEffect.h"
#include "Entities/Populations/Actions/PHY_FireResults_Population.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Units/Humans/MIL_Injury_ABC.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "Entities/Objects/AttritionCapacity.h"
#include "Entities/Effects/MIL_Effect_PopulationFire.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/MIL_Army.h"
#include "MIL_AgentServer.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_World.h"
#include <boost/serialization/vector.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationElement_ABC::MIL_PopulationElement_ABC( MIL_Population& population, unsigned int nID )
    : pPopulation_     ( &population )
    , nID_             ( nID )
    , rDensity_        ( 0. )
    , pAttitude_       ( &population.GetDefaultAttitude() )
    , bAttitudeUpdated_( true )
    , bHumansUpdated_  ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationElement_ABC::~MIL_PopulationElement_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
double MIL_PopulationElement_ABC::GetDangerosity( const MIL_AgentPion& target ) const
{
    assert( pAttitude_   );
    assert( pPopulation_ );
    if( target.GetRole< PHY_RoleInterface_Population >().IsInvulnerable() )
        return 0.;

    const PHY_ComposantePion* pTargetComposante = target.GetRole< PHY_RolePion_Composantes >().GetMajorComposante();
    if( !pTargetComposante )
        return 0.;

    const double rPH = pPopulation_->GetType().GetPH( *pAttitude_, rDensity_ );
    const PHY_AttritionData attritionData = pPopulation_->GetType().GetAttritionData( *pAttitude_, pTargetComposante->GetType().GetProtection(), pPopulation_->GetArmedIndividuals() );

    return rPH * attritionData.GetScore();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::FireOnPion
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::FireOnPion( double rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult )
{
    assert( pAttitude_ );
    assert( pPopulation_ );
    if( target.GetRole< PHY_RoleInterface_Population >().IsInvulnerable() )
        return;

    PHY_Composante_ABC::T_ComposanteVector compTargets;
    target.GetRole< PHY_RoleInterface_Composantes >().GetComposantesAbleToBeFired( compTargets );
    if( compTargets.empty() )
        return;

    target.NotifyAttackedBy( *pPopulation_ );

    const double rPH = GetPopulation().GetType().GetPH( *pAttitude_, rDensity_ );
    if( !( 1. - MIL_Random::rand_io( MIL_Random::eFire ) <= rPH * rIntensity ) )
        return;

    MIL_Effect_PopulationFire* pEffect = new MIL_Effect_PopulationFire( GetPopulation().GetType(), GetAttitude(), target, *compTargets.front(), fireResult, GetPopulation().GetArmedIndividuals() );
    MIL_EffectManager::GetEffectManager().Register( *pEffect );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::FireOnPions
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::FireOnPions( double rIntensity, PHY_FireResults_Population& fireResult )
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
void MIL_PopulationElement_ABC::ApplyFire( unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult, bool lethal )
{
    assert( pPopulation_ );
    unsigned int nHit = std::min( humans_.GetTotalLivingHumans(), nNbrAmmoFired );
    if( nHit == 0 )
        return;
    bHumansUpdated_ = true;
    if( lethal )
        ApplyLethalDamage( nHit, fireResult );
    else
    {
        bHumansUpdated_ = true;
        PullHumans( nHit );
        fireResult.GetDamages( *pPopulation_ ).NotifyHumansScattered( nHit );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ApplyLethalDamage
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ApplyLethalDamage( unsigned int nHit, PHY_FireResults_ABC& fireResult )
{
    bHumansUpdated_ = true;
    humans_.ApplyNumberOfDead( nHit );
    unsigned int nWounds = static_cast< unsigned >( nHit * MIL_Random::rand_ii( 0.6, 1.25, MIL_Random::eWounds ) );
    humans_.ApplyWounds( nWounds );
    fireResult.GetDamages( *pPopulation_ ).NotifyHumansKilled( nHit );
    fireResult.GetDamages( *pPopulation_ ).NotifyHumansWounded( nWounds );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ApplyIndirectFire
// Created: SBO 2006-01-19
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ApplyIndirectFire( const MT_Circle& attritionCircle, PHY_FireResults_ABC& fireResult )
{
    assert( pPopulation_ );
    double rDead = std::min( static_cast< double >( humans_.GetTotalLivingHumans() ), rDensity_ * GetLocation().GetIntersectionAreaWithCircle( attritionCircle ) );
    // $$$$ SBO 2006-04-07: 2% kill, at least one kill
    unsigned int nDead = static_cast< unsigned int >( ceil( 0.02f * rDead ) );
    ApplyLethalDamage( nDead, fireResult );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ApplyExplosion
// Created: NLD 2006-04-24
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ApplyExplosion( const AttritionCapacity& capacity, PHY_FireResults_ABC& fireResult )
{
    assert( pPopulation_ );
    const unsigned int nNbrTarget = std::min( humans_.GetTotalLivingHumans(), static_cast< unsigned int >( std::max( 1., rDensity_ * capacity.GetAttritionSurface() ) ) );
    const double rPH = capacity.GetAttritionPH();
    unsigned int nHit = 0;
    for( unsigned int i = 0; i < nNbrTarget; ++i )
        if( 1. - MIL_Random::rand_io( MIL_Random::eFire ) <= rPH )
            ++nHit;
    ApplyLethalDamage( nHit, fireResult );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ApplyExplosion
// Created: NLD 2006-04-24
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ApplyInjury( MIL_Injury_ABC& injury )
{
    injury.SetInjury( humans_.GetTotalLivingHumans(), rDensity_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ApplyBurn
// Created: BCI 2010-12-15
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ApplyBurn( const MIL_BurnEffectManipulator& /*burn*/ )
{
    // NOTHING
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
    TER_ObjectManager::T_ObjectVector objects;
    TER_World::GetWorld().GetObjectManager().GetListWithinLocalisation( GetLocation(), objects );
    for( TER_Object_ABC::CIT_ObjectVector it = objects.begin(); it != objects.end(); ++it )
    {
        MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **it );
        object.NotifyPopulationMovingInside( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetPionMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
double MIL_PopulationElement_ABC::GetPionMaxSpeed( const PHY_Volume& pionVolume ) const
{
    assert( pAttitude_ );
    return pPopulation_->GetPionMaxSpeed( *pAttitude_, humans_.GetLivingRatio() * GetDefaultDensity( pPopulation_->GetType() ), pionVolume );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetPionReloadingTimeFactor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
double MIL_PopulationElement_ABC::GetPionReloadingTimeFactor() const
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
    GetLocation().ComputeNearestPoint( securingAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::load
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> humans_
         >> rDensity_
         >> collidingAgents_;
    unsigned int nAttitudeID;
    file >> nAttitudeID;
    pAttitude_ = MIL_PopulationAttitude::Find( nAttitudeID );
    assert( pAttitude_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned attitude = pAttitude_->GetID();
    file << humans_
         << rDensity_
         << collidingAgents_
         << attitude;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetDensity
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
double MIL_PopulationElement_ABC::GetDensity() const
{
    return rDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetAttitude
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude& MIL_PopulationElement_ABC::GetAttitude() const
{
    assert( pAttitude_ );
    return *pAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::Clean()
{
    bAttitudeUpdated_ = false;
    bHumansUpdated_   = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::HasHumansChanged
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
bool MIL_PopulationElement_ABC::HasHumansChanged() const
{
    return bHumansUpdated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::HasAttitudeChanged
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
bool MIL_PopulationElement_ABC::HasAttitudeChanged() const
{
    return bAttitudeUpdated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetPopulation
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
MIL_Population& MIL_PopulationElement_ABC::GetPopulation() const
{
    assert( pPopulation_ );
    return *pPopulation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetID
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationElement_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetAllHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationElement_ABC::GetAllHumans() const
{
    return humans_.GetAllHumans();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetTotalLivingHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationElement_ABC::GetTotalLivingHumans() const
{
    return humans_.GetTotalLivingHumans();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetHealthyHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationElement_ABC::GetHealthyHumans() const
{
    return humans_.GetHealthyHumans();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetWoundedHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationElement_ABC::GetWoundedHumans() const
{
    return humans_.GetWoundedHumans();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetContaminatedHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationElement_ABC::GetContaminatedHumans() const
{
    return humans_.GetContaminatedHumans();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetDeadHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationElement_ABC::GetDeadHumans() const
{
    return humans_.GetDeadHumans();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::UpdateDensity
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::UpdateDensity()
{
    const double rArea = GetLocation().GetArea();
    if( rArea == 0. )
        rDensity_ = 0; // $$$ +infini sauf si aucun humain
    else
        rDensity_ = static_cast< double >( humans_.GetTotalLivingHumans() ) / rArea;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::SetAttitude
// Created: SBO 2005-10-25
// ----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::SetAttitude( const MIL_PopulationAttitude& attitude )
{
    if( pAttitude_ != &attitude )
    {
        bAttitudeUpdated_ = true;
        pAttitude_ = &attitude;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::PushHumans
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::PushHumans( const MIL_PopulationHumans& humans )
{
    if( humans.IsEmpty() )
        return;
    bHumansUpdated_ = true;
    humans_ += humans;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::PullHumans
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationHumans MIL_PopulationElement_ABC::PullHumans( unsigned int nNbr )
{
    MIL_PopulationHumans humans;
    humans.PullFrom( nNbr, humans_ );
    if( !humans.IsEmpty() )
        bHumansUpdated_ = true;
    return humans;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::KillAllHumans
// Created: NLD 2005-12-06
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::KillAllHumans()
{
    if( humans_.GetTotalLivingHumans() == 0. )
        return;
    humans_.KillAll();
    bHumansUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ReintegrateUrbanBlock
// Created: BCI 2011-03-21
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ReintegrateUrbanBlock()
{
    humans_.ReintegrateUrbanBlock();
    bHumansUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::Exterminate
// Created: SBO 2005-12-22
// -----------------------------------------------------------------------------
double MIL_PopulationElement_ABC::Exterminate( double rSurface )
{
    assert( rDensity_ != 0 );
    unsigned int nKills = static_cast< unsigned int >( rSurface * rDensity_ );
    nKills = humans_.ApplyNumberOfDead( nKills );
    bHumansUpdated_ = true;
    return nKills / rDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::IsDead
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
bool MIL_PopulationElement_ABC::IsDead() const
{
    return humans_.GetTotalLivingHumans() == 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::IsInZone
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
bool MIL_PopulationElement_ABC::IsInZone( const TER_Localisation& loc ) const
{
    return GetLocation().IsIntersecting( loc );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ClearCollisions
// Created: NLD 2005-12-06
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ClearCollisions()
{
    collidingAgents_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ApplyContamination
// Created: LGY 2011-03-30
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ApplyContamination( const MIL_NbcAgentType& type )
{
    const MIL_PopulationType& populationType = pPopulation_->GetType();
    IT_ContaminationEffects it = contaminationEffects_.find( type.GetID() );
    if( it == contaminationEffects_.end() )
    {
        boost::shared_ptr< MIL_ContaminationEffect > pEffect( new MIL_ContaminationEffect( humans_, static_cast< unsigned int >( populationType.GetDelay() ), MIL_AgentServer::GetWorkspace().GetRealTime() ) );
        contaminationEffects_[ type.GetID() ] = pEffect;
    }
    else
        it->second->Update( MIL_AgentServer::GetWorkspace().GetRealTime() );
    bHumansUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ApplyIntoxication
// Created: LGY 2011-03-31
// -----------------------------------------------------------------------------
void MIL_PopulationElement_ABC::ApplyIntoxication( const MIL_NbcAgentType& type )
{
    const MIL_PopulationType& populationType = pPopulation_->GetType();
    IT_IntoxicationEffects it = intoxicationEffects_.find( type.GetID() );
    if( it == intoxicationEffects_.end() )
    {
        boost::shared_ptr< MIL_IntoxicationEffect > pEffect( new MIL_IntoxicationEffect( humans_, static_cast< unsigned int >( populationType.GetDelay() ), MIL_AgentServer::GetWorkspace().GetRealTime() ) );
        type.InitializePopulationEffect( *pEffect );
        intoxicationEffects_[ type.GetID() ] = pEffect;
    }
    else
        it->second->Update( MIL_AgentServer::GetWorkspace().GetRealTime() );
    bHumansUpdated_ = true;
}
