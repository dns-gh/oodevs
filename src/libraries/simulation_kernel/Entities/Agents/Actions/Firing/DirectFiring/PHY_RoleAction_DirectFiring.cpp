// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 16:03 $
// $Revision: 9 $
// $Workfile: PHY_RoleAction_DirectFiring.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_RoleAction_DirectFiring.h"

#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/ControlZoneCapacity.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Actions/PHY_FireResults_Default.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/MIL_Knowledgegroup.h"

#include "simulation_kernel/WeaponAvailabilityComputer_ABC.h"
#include "simulation_kernel/WeaponAvailabilityComputerFactory_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_DirectFiring, "PHY_RoleAction_DirectFiring" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RoleAction_DirectFiring* role, const unsigned int /*version*/ )
{
    const firing::WeaponAvailabilityComputerFactory_ABC* const factory = &role->weaponAvailabilityComputerFactory_;
    archive << role->pPion_
            << factory;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RoleAction_DirectFiring* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    firing::WeaponAvailabilityComputerFactory_ABC* weaponAvailabilityComputerFactory;
    archive >> pion
            >> weaponAvailabilityComputerFactory;
    ::new( role )PHY_RoleAction_DirectFiring( *pion, *weaponAvailabilityComputerFactory );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_DirectFiring::PHY_RoleAction_DirectFiring( MIL_AgentPion& pion, const firing::WeaponAvailabilityComputerFactory_ABC& weaponAvailabilityComputerFactory )
    : pPion_     ( &pion )
    , weaponAvailabilityComputerFactory_ ( weaponAvailabilityComputerFactory )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_DirectFiring::~PHY_RoleAction_DirectFiring()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_DirectFiring::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< tools::Role_ABC >( *this )
			& pPion_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::GetPopulationTarget
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_Population* PHY_RoleAction_DirectFiring::GetPopulationTarget( uint nTargetKnowledgeID )
{
    assert( pPion_ );
    
    DEC_Knowledge_Population* pKnowledge = pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( nTargetKnowledgeID );
    return pKnowledge ? &pKnowledge->GetPopulationKnown() : 0;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePion
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::FirePion( PHY_DirectFireData& firerWeapons, MIL_Agent_ABC& target, const PHY_RoleInterface_Composantes::T_ComposanteVector& compTargets, PHY_FireResults_Pion& fireResult )
{
    assert( pPion_ );

    // Pour chaque cible, choix de la meilleure arme
    uint nNbrWeaponsUsed = 0;
    for( PHY_RoleInterface_Composantes::CIT_ComposanteVector itCompTarget = compTargets.begin(); itCompTarget != compTargets.end(); ++itCompTarget )
    {
        PHY_Composante_ABC& compTarget       = **itCompTarget;
        const PHY_ComposantePion* pBestFirer       = 0;
        PHY_Weapon*         pBestFirerWeapon = 0;

        firerWeapons.ChooseBestWeapon( target, compTarget, pBestFirer, pBestFirerWeapon );
        if( !pBestFirer )
            continue;

        assert( pBestFirerWeapon );
        pBestFirerWeapon->DirectFire( *pPion_, target, compTarget, fireResult, true ); // 'true' is for 'use ph'
        ++nNbrWeaponsUsed;

        firerWeapons.ReleaseWeapon( *pBestFirer, *pBestFirerWeapon );
    }

    // Pour toutes les armes non utilis�es, choix de la meilleure cible
    const PHY_ComposantePion* pUnusedFirer       = 0;
    PHY_Weapon*               pUnusedFirerWeapon = 0;
    while( firerWeapons.GetUnusedFirerWeapon( pUnusedFirer, pUnusedFirerWeapon ) && nNbrWeaponsUsed < compTargets.size() )
    {
        PHY_Composante_ABC*  pBestCompTarget = 0;
        MT_Float             rBestScore      = 0.;
        for( PHY_RoleInterface_Composantes::CIT_ComposanteVector itCompTarget = compTargets.begin(); itCompTarget != compTargets.end(); ++itCompTarget )
        {
            PHY_Composante_ABC& compTarget = **itCompTarget;

            const MT_Float rCurrentScore = pUnusedFirerWeapon->GetDangerosity( *pPion_, target, compTarget.GetType(), true ); // 'true' is for 'use ph'
            if( rCurrentScore > rBestScore )
            {
                rBestScore      = rCurrentScore;
                pBestCompTarget = &compTarget;
            }
        }

        if( pBestCompTarget )
        {
            pUnusedFirerWeapon->DirectFire( *pPion_, target, *pBestCompTarget, fireResult, true ); // 'true' is for 'use ph'
            ++nNbrWeaponsUsed;
        }

        firerWeapons.ReleaseWeapon( *pUnusedFirer, *pUnusedFirerWeapon );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePion
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
int PHY_RoleAction_DirectFiring::FirePion( DEC_Knowledge_Agent* pEnemy, PHY_DirectFireData::E_FiringMode nFiringMode, MT_Float rPercentageComposantesToUse, PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType, PHY_DirectFireData::E_ComposanteFiredType nComposanteFiredType, PHY_FireResults_Pion*& pFireResult, const PHY_AmmoDotationClass* pAmmoDotationClass /* =0 */  )
{
    assert( pPion_ );
    MIL_Agent_ABC* pTarget = pEnemy ? &pEnemy->GetAgentKnown() : 0;
    if( !pTarget )
        return eImpossible;

    if( pTarget->IsDead() )
        return eEnemyDestroyed;

    // Firers
    PHY_DirectFireData firerWeapons( *pPion_, nComposanteFiringType, nFiringMode, rPercentageComposantesToUse, pAmmoDotationClass );
    
    std::auto_ptr< firing::WeaponAvailabilityComputer_ABC > algorithm( weaponAvailabilityComputerFactory_.Create( firerWeapons) );
    pPion_->Execute( *algorithm );

    const uint nNbrWeaponsUsable = firerWeapons.GetNbrWeaponsUsable();
    if( nNbrWeaponsUsable == 0 )
    {
        if( firerWeapons.HasWeaponsNotReady() )
            return eRunning;
        if( firerWeapons.HasWeaponsAndNoAmmo() )
            return eNoAmmo;
        return eNoCapacity;
    }
    
    pPion_ ->NotifyAttacking ( *pTarget );
    pTarget->NotifyAttackedBy( *pPion_  );

    // Targets
    PHY_RoleInterface_Composantes::T_ComposanteVector targets;
    const bool bFireOnlyOnMajorComposantes = ( nComposanteFiredType == PHY_DirectFireData::eFireOnlyOnMajorComposantes );
    pTarget->GetRole< PHY_RoleInterface_Composantes >().GetComposantesAbleToBeFired( targets, nNbrWeaponsUsable, bFireOnlyOnMajorComposantes );
    if( targets.empty() )
        return eEnemyDestroyed;

    assert( targets.size() == nNbrWeaponsUsable );    

    if( !pFireResult )
        pFireResult = new PHY_FireResults_Pion( *pPion_, *pTarget );

    FirePion( firerWeapons, *pTarget, targets, *pFireResult );
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePionSuspended
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::FirePionSuspended( DEC_Knowledge_Agent* pEnemy )
{
    MIL_Agent_ABC* pTarget = pEnemy ? &pEnemy->GetAgentKnown() : 0;
    if ( pTarget )
    {
        assert( pPion_ );
        pTarget->NotifyAttackedBy( *pPion_ );
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

    assert( pPion_ );    
    typedef std::pair< MIL_Agent_ABC*, PHY_Composante_ABC* > T_TargetPair;
    typedef std::vector< T_TargetPair >                      T_TargetVector;
    typedef T_TargetVector::const_iterator                   CIT_TargetVector;
    
    T_TargetVector targets;
    const ControlZoneCapacity* capacity = object.Retrieve< ControlZoneCapacity >();
    if ( capacity )
        capacity->RetrieveTargets( object, targets );
    
    PHY_DirectFireData firerWeapons( *pPion_, PHY_DirectFireData::eFireUsingOnlyComposantesLoadable, PHY_DirectFireData::eFiringModeNormal );
    pPion_->GetRole< PHY_RolePion_Composantes >().ApplyOnWeapons( firerWeapons );

    for( CIT_TargetVector itTarget = targets.begin(); itTarget != targets.end(); ++itTarget )
    {
        MIL_Agent_ABC&      target       = *itTarget->first;
        PHY_Composante_ABC& compTarget   = *itTarget->second;
        const PHY_ComposantePion* pCompFirer   = 0;
        PHY_Weapon*         pFirerWeapon = 0;

        firerWeapons.ChooseRandomWeapon( target, compTarget, pCompFirer, pFirerWeapon );
        if( !pCompFirer )
            continue;

        assert( pFirerWeapon );
        pFirerWeapon->DirectFire( *pPion_, *itTarget->first, *itTarget->second, *pFireResult, false ); // 'false' is for 'don't use ph'
        
        firerWeapons.ReleaseWeapon( *pCompFirer, *pFirerWeapon );
    } 
}

// =============================================================================
// FIRE POPULATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePopulation
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_DirectFiring::FirePopulation( uint nTargetKnowledgeID, PHY_FireResults_Pion*& pFireResult )
{
    assert( pPion_ );
    MIL_Population* pTarget = GetPopulationTarget( nTargetKnowledgeID );
    if( !pTarget )
        return eImpossible;

    if( pTarget->IsDead() )
        return eEnemyDestroyed;

    MIL_PopulationElement_ABC* pPopulationElement = pTarget->GetClosestAliveElement( *pPion_ );
    if( !pPopulationElement )
        return eEnemyDestroyed;
   
    // Firers
    PHY_DirectFireData firerWeapons( *pPion_, PHY_DirectFireData::eFireUsingAllComposantes, PHY_DirectFireData::eFiringModeNormal, 1., &PHY_AmmoDotationClass::mitraille_ );
    pPion_->GetRole< PHY_RolePion_Composantes >().ApplyOnWeapons( firerWeapons );

    const uint nNbrWeaponsUsable = firerWeapons.GetNbrWeaponsUsable();
    if( nNbrWeaponsUsable == 0 )
    {
        if( firerWeapons.HasWeaponsNotReady() )
            return eRunning;
        if( firerWeapons.HasWeaponsAndNoAmmo() )
            return eNoAmmo;
        return eNoCapacity;
    }

    pPion_ ->NotifyAttacking ( *pTarget );
    pTarget->NotifyAttackedBy( *pPion_  );

    if( !pFireResult )
        pFireResult = new PHY_FireResults_Pion( *pPion_, *pTarget );

    // Tir
    const PHY_ComposantePion* pFirer       = 0;
    PHY_Weapon*         pFirerWeapon = 0;
    while( firerWeapons.GetUnusedFirerWeapon( pFirer, pFirerWeapon ) )
    {
        pFirerWeapon->DirectFire( *pPion_, *pPopulationElement, *pFireResult ); 
        firerWeapons.ReleaseWeapon( *pFirer, *pFirerWeapon );
    }

    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FirePopulationSuspended
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::FirePopulationSuspended( uint nTargetKnowledgeID )
{
    MIL_Population* pTarget = GetPopulationTarget( nTargetKnowledgeID );
    if ( pTarget )
    {
        assert( pPion_ );
        pTarget->NotifyAttackedBy( *pPion_ );
    }
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
