// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_DirectFiring.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Illumination/PHY_RoleInterface_Illumination.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/ControlZoneCapacity.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Actions/PHY_FireResults_Default.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/MIL_Knowledgegroup.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/ConsumeDotationNotificationHandler_ABC.h"
#include "simulation_kernel/ComposantesAbleToBeFiredComputer_ABC.h"
#include "simulation_kernel/ComposantesAbleToBeFiredComputerFactory_ABC.h"
#include "simulation_kernel/WeaponAvailabilityComputer_ABC.h"
#include "simulation_kernel/WeaponAvailabilityComputerFactory_ABC.h"

using namespace firing;

BOOST_CLASS_EXPORT_IMPLEMENT( firing::PHY_RoleAction_DirectFiring )

template< typename Archive >
void firing::save_construct_data( Archive& archive, const PHY_RoleAction_DirectFiring* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void firing::load_construct_data( Archive& archive, PHY_RoleAction_DirectFiring* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RoleAction_DirectFiring( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_DirectFiring::PHY_RoleAction_DirectFiring( MIL_AgentPion& pion )
    : pion_( pion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_DirectFiring::~PHY_RoleAction_DirectFiring()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_DirectFiring::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< tools::Role_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::GetPopulationTarget
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_Population* PHY_RoleAction_DirectFiring::GetPopulationTarget( unsigned int nTargetKnowledgeID )
{
    DEC_Knowledge_Population* pKnowledge = pion_.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( nTargetKnowledgeID );
    return pKnowledge ? &pKnowledge->GetPopulationKnown() : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePion
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::FirePion( PHY_DirectFireData& firerWeapons, MIL_Agent_ABC& target, const PHY_Composante_ABC::T_ComposanteVector& compTargets, PHY_FireResults_Pion& fireResult )
{
    // Pour chaque cible, choix de la meilleure arme
    unsigned int nNbrWeaponsUsed = 0;
    for( PHY_Composante_ABC::CIT_ComposanteVector itCompTarget = compTargets.begin(); itCompTarget != compTargets.end(); ++itCompTarget )
    {
        PHY_Composante_ABC& compTarget       = **itCompTarget;
        const PHY_ComposantePion* pBestFirer       = 0;
        PHY_Weapon*         pBestFirerWeapon = 0;

        firerWeapons.ChooseBestWeapon( target, compTarget, pBestFirer, pBestFirerWeapon );
        if( !pBestFirer )
            continue;
        assert( pBestFirerWeapon );
        pBestFirerWeapon->DirectFire( pion_, target, compTarget, fireResult, true ); // 'true' is for 'use ph'
        ++nNbrWeaponsUsed;
        firerWeapons.ReleaseWeapon( *pBestFirer, *pBestFirerWeapon );
    }
    // Pour toutes les armes non utilisées, choix de la meilleure cible
    const PHY_ComposantePion* pUnusedFirer       = 0;
    PHY_Weapon*               pUnusedFirerWeapon = 0;
    while( firerWeapons.GetUnusedFirerWeapon( pUnusedFirer, pUnusedFirerWeapon ) && nNbrWeaponsUsed < compTargets.size() )
    {
        PHY_Composante_ABC* pBestCompTarget = 0;
        double rBestScore = 0.;
        for( PHY_Composante_ABC::CIT_ComposanteVector itCompTarget = compTargets.begin(); itCompTarget != compTargets.end(); ++itCompTarget )
        {
            PHY_Composante_ABC& compTarget = **itCompTarget;
            const double rCurrentScore = pUnusedFirerWeapon->GetDangerosity( pion_, target, compTarget.GetType(), true ); // 'true' is for 'use ph'
            if( rCurrentScore > rBestScore )
            {
                rBestScore = rCurrentScore;
                pBestCompTarget = &compTarget;
            }
        }
        if( pBestCompTarget )
        {
            pUnusedFirerWeapon->DirectFire( pion_, target, *pBestCompTarget, fireResult, true ); // 'true' is for 'use ph'
            ++nNbrWeaponsUsed;
        }
        firerWeapons.ReleaseWeapon( *pUnusedFirer, *pUnusedFirerWeapon );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePion
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
int PHY_RoleAction_DirectFiring::FirePion( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, PHY_DirectFireData::E_FiringMode nFiringMode, double rPercentageComposantesToUse, PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType, PHY_DirectFireData::E_ComposanteFiredType nComposanteFiredType, PHY_FireResults_Pion*& pFireResult, const PHY_AmmoDotationClass* pAmmoDotationClass /* =0 */  )
{
    MIL_Agent_ABC* pTarget = pEnemy && pEnemy->IsValid() ? &pEnemy->GetAgentKnown() : 0;
    if( !pTarget )
        return eImpossible;
    if( pTarget->IsDead() )
        return eEnemyDestroyed;
    // Firers
    PHY_DirectFireData firerWeapons( pion_, nComposanteFiringType, nFiringMode, rPercentageComposantesToUse, pAmmoDotationClass );
    std::auto_ptr< WeaponAvailabilityComputer_ABC > weaponAvailabilityComputer = pion_.GetAlgorithms().weaponAvailabilityComputerFactory_->Create( firerWeapons );
    pion_.Execute( *weaponAvailabilityComputer );
    const unsigned int nNbrWeaponsUsable = firerWeapons.GetNbrWeaponsUsable();
    if( nNbrWeaponsUsable == 0 )
    {
        if( firerWeapons.HasWeaponsNotReady() )
            return eRunning;
        if( firerWeapons.HasWeaponsAndNoAmmo() )
            return eNoAmmo;
        return eNoCapacity;
    }
    pion_.NotifyAttacking ( *pTarget );
    pTarget->NotifyAttackedBy( pion_  );
    // Targets
    const bool bFireOnlyOnMajorComposantes = ( nComposanteFiredType == PHY_DirectFireData::eFireOnlyOnMajorComposantes );
    std::auto_ptr< ComposantesAbleToBeFiredComputer_ABC > componentAbleToBeFiredComputer = pion_.GetAlgorithms().composantesAbleToBeFiredComputerFactory_->Create( bFireOnlyOnMajorComposantes );
    pTarget->Execute< OnComponentComputer_ABC >( *componentAbleToBeFiredComputer );
    PHY_Composante_ABC::T_ComposanteVector& targets = componentAbleToBeFiredComputer->ResultLimited( nNbrWeaponsUsable );
    if( targets.empty() )
        return eEnemyDestroyed;
    assert( targets.size() == nNbrWeaponsUsable );
    if( !pFireResult )
        pFireResult = new PHY_FireResults_Pion( pion_, *pTarget );
    FirePion( firerWeapons, *pTarget, targets, *pFireResult );
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePionSuspended
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::FirePionSuspended( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy )
{
    if( pEnemy && pEnemy->IsValid() )
        pEnemy->GetAgentKnown().NotifyAttackedBy( pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::IlluminatePion
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
int  PHY_RoleAction_DirectFiring::IlluminatePion( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy )
{
    MIL_Agent_ABC* pTarget = pEnemy && pEnemy->IsValid() ? &pEnemy->GetAgentKnown() : 0;
    if( !pTarget )
        return eImpossible;
    if( pTarget->IsDead() )
        return eEnemyDestroyed;
    if( pTarget->GetRole< PHY_RoleInterface_Illumination >().IsUnderIndirectFire() )
        return eFinished;
    const dotation::PHY_RoleInterface_Dotations& roleDotations = pion_.GetRole< dotation::PHY_RoleInterface_Dotations >();
    const PHY_RoleInterface_Location& pionLocation = pion_.Get< PHY_RoleInterface_Location >();
    double range = pionLocation.GetPosition().Distance( pEnemy->GetPosition() );
    const PHY_DotationCategory* munition = roleDotations.GetIlluminationDotations( (float)range , true );
    if( munition  )
    {
        pTarget->GetRole< PHY_RoleInterface_Illumination >().NotifyDefinitelyIlluminated();
        unsigned int consommation = 1;
        pion_.Apply( &dotation::ConsumeDotationNotificationHandler_ABC::NotifyConsumeDotation, *munition, consommation );
        return eFinished;
    }
    munition = roleDotations.GetIlluminationDotations( (float)range, false );
    if( munition )
    {
        pTarget->GetRole< PHY_RoleInterface_Illumination >().NotifyStartIlluminatedBy( pion_ );
        pion_.GetRole< PHY_RoleInterface_Illumination >().NotifyStartIlluminate( *pTarget );
        return eRunning;
    }
    else
        return eNoCapacity;
}
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::IlluminatePionSuspended
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::IlluminatePionSuspended( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy )
{
    MIL_Agent_ABC* pTarget = pEnemy && pEnemy->IsValid() ? &pEnemy->GetAgentKnown() : 0;
    if( !pTarget )
    {
        pTarget->GetRole< PHY_RoleInterface_Illumination >().NotifyStopIlluminatedBy( pion_ );
        pion_.GetRole< PHY_RoleInterface_Illumination >().NotifyStopIlluminate();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FireZone
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::FireZone( const MIL_Object_ABC& object, PHY_FireResults_Default*& pFireResult )
{
    if( !pFireResult )
        pFireResult = new PHY_FireResults_Default();
    typedef std::pair< MIL_Agent_ABC*, PHY_Composante_ABC* >   T_TargetPair;
    typedef std::vector< T_TargetPair >                        T_TargetVector;
    typedef T_TargetVector::const_iterator                   CIT_TargetVector;
    T_TargetVector targets;
    const ControlZoneCapacity* capacity = object.Retrieve< ControlZoneCapacity >();
    if( capacity )
        capacity->RetrieveTargets( object, targets );
    PHY_DirectFireData firerWeapons( pion_, PHY_DirectFireData::eFireUsingOnlyComposantesLoadable, PHY_DirectFireData::eFiringModeNormal );
    std::auto_ptr< WeaponAvailabilityComputer_ABC > weaponAvailabilityComputer( pion_.GetAlgorithms().weaponAvailabilityComputerFactory_->Create( firerWeapons ) );
    pion_.Execute( *weaponAvailabilityComputer );
    for( CIT_TargetVector itTarget = targets.begin(); itTarget != targets.end(); ++itTarget )
    {
        MIL_Agent_ABC& target = *itTarget->first;
        PHY_Composante_ABC& compTarget = *itTarget->second;
        const PHY_ComposantePion* pCompFirer = 0;
        PHY_Weapon*  pFirerWeapon = 0;
        firerWeapons.ChooseRandomWeapon( target, compTarget, pCompFirer, pFirerWeapon );
        if( !pCompFirer )
            continue;
        assert( pFirerWeapon );
        pFirerWeapon->DirectFire( pion_, *itTarget->first, *itTarget->second, *pFireResult, false ); // 'false' is for 'don't use ph'
        firerWeapons.ReleaseWeapon( *pCompFirer, *pFirerWeapon );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePopulation
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_DirectFiring::FirePopulation( unsigned int nTargetKnowledgeID, PHY_FireResults_Pion*& pFireResult, const PHY_AmmoDotationClass* dotationClass )
{
    MIL_Population* pTarget = GetPopulationTarget( nTargetKnowledgeID );
    if( !pTarget )
        return eImpossible;
    if( pTarget->IsDead() )
        return eEnemyDestroyed;
    MIL_PopulationElement_ABC* pPopulationElement = pTarget->GetClosestAliveElement( pion_ );
    if( !pPopulationElement )
        return eEnemyDestroyed;
    // Firers
    PHY_DirectFireData firerWeapons( pion_, PHY_DirectFireData::eFireUsingAllComposantes, PHY_DirectFireData::eFiringModeNormal, 1., dotationClass );
    std::auto_ptr< WeaponAvailabilityComputer_ABC > weaponAvailabilityComputer( pion_.GetAlgorithms().weaponAvailabilityComputerFactory_->Create( firerWeapons ) );
    pion_.Execute( *weaponAvailabilityComputer );
    const unsigned int nNbrWeaponsUsable = firerWeapons.GetNbrWeaponsUsable();
    if( nNbrWeaponsUsable == 0 )
    {
        if( firerWeapons.HasWeaponsNotReady() )
            return eRunning;
        if( firerWeapons.HasWeaponsAndNoAmmo() )
            return eNoAmmo;
        return eNoCapacity;
    }
    pion_.NotifyAttacking ( *pTarget );
    pTarget->NotifyAttackedBy( pion_  );
    if( !pFireResult )
        pFireResult = new PHY_FireResults_Pion( pion_, *pTarget );
    // Tir
    const PHY_ComposantePion* pFirer = 0;
    PHY_Weapon* pFirerWeapon = 0;
    while( firerWeapons.GetUnusedFirerWeapon( pFirer, pFirerWeapon ) )
    {
        pFirerWeapon->DirectFire( pion_, *pPopulationElement, *pFireResult );
        firerWeapons.ReleaseWeapon( *pFirer, *pFirerWeapon );
    }
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePopulationSuspended
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::FirePopulationSuspended( unsigned int nTargetKnowledgeID )
{
    MIL_Population* pTarget = GetPopulationTarget( nTargetKnowledgeID );
    if( pTarget )
        pTarget->NotifyAttackedBy( pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::GetInitialReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
int PHY_RoleAction_DirectFiring::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::GetFinalReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
int PHY_RoleAction_DirectFiring::GetFinalReturnCode() const
{
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::Update
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::Clean
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::Clean()
{
    // NOTHING
}
