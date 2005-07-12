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

#include "MIL_Pch.h"

#include "PHY_RoleAction_DirectFiring.h"

#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KS_AgentQuerier.h"
#include "PHY_DirectFireResults.h"
#include "PHY_ControlZoneFireResults.h"
#include "Entities/Objects/MIL_ControlZone.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_DirectFiring, "PHY_RoleAction_DirectFiring" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_DirectFiring::PHY_RoleAction_DirectFiring( MT_RoleContainer& role, MIL_AgentPion& pion )
    : MT_Role_ABC( role )
    , pPion_     ( &pion )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_DirectFiring::PHY_RoleAction_DirectFiring()
    : MT_Role_ABC()
    , pPion_     ( 0 )
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
    archive & boost::serialization::base_object< MT_Role_ABC >( *this )
            & pPion_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::GetTarget
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_Agent_ABC* PHY_RoleAction_DirectFiring::GetTarget( uint nTargetKnowledgeID )
{
    assert( pPion_ );
    
    DEC_Knowledge_Agent* pKnowledge = pPion_->GetKSQuerier().GetKnowledgeAgentFromDiaID( nTargetKnowledgeID );
    return pKnowledge ? &pKnowledge->GetAgentKnown() : 0;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::Fire
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::Fire( PHY_DirectFireData& firerWeapons, MIL_Agent_ABC& target, const PHY_RoleInterface_Composantes::T_ComposanteVector& compTargets, E_Mode nFireMode, PHY_DirectFireResults& fireResult )
{
    assert( pPion_ );

    // Pour chaque cible, choix de la meilleure arme
    uint nNbrWeaponsUsed = 0;
    for( PHY_RoleInterface_Composantes::CIT_ComposanteVector itCompTarget = compTargets.begin(); itCompTarget != compTargets.end(); ++itCompTarget )
    {
        PHY_Composante_ABC& compTarget       = **itCompTarget;
        PHY_ComposantePion* pBestFirer       = 0;
        PHY_Weapon*         pBestFirerWeapon = 0;

        firerWeapons.ChooseBestWeapon( target, compTarget, pBestFirer, pBestFirerWeapon );
        if( !pBestFirer )
            continue;

        assert( pBestFirerWeapon );
        pBestFirerWeapon->DirectFire( *pPion_, target, compTarget, fireResult, true ); // 'true' is for 'use ph'
        ++nNbrWeaponsUsed;

        switch( nFireMode )
        {
            case eModeNormal: firerWeapons.RemoveFirer ( *pBestFirer ); break;
            case eModeFree  : firerWeapons.RemoveWeapon( *pBestFirer, *pBestFirerWeapon ); break;
            default:
                assert( false );
        }
    }

    // Pour toutes les armes non utilisées, choix de la meilleure cible
    PHY_ComposantePion* pUnusedFirer       = 0;
    PHY_Weapon*         pUnusedFirerWeapon = 0;
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
        
        switch( nFireMode )
        {
            case eModeNormal: firerWeapons.RemoveFirer ( *pUnusedFirer ); break;
            case eModeFree  : firerWeapons.RemoveWeapon( *pUnusedFirer, *pUnusedFirerWeapon ); break;
            default:
                assert( false );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::Fire
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
int PHY_RoleAction_DirectFiring::Fire( uint nTargetKnowledgeID, E_Mode nFireMode, MT_Float rPercentageComposantesToUse, PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType, PHY_DirectFireResults*& pFireResult )
{
    MIL_Agent_ABC* pTarget = GetTarget( nTargetKnowledgeID );
    if( !pTarget )
        return eImpossible;

    if( pTarget->IsDead() )
        return eEnemyDestroyed;

    assert( pPion_ );
    
    if( !pFireResult )
        pFireResult = new PHY_DirectFireResults( *pPion_, *pTarget );

    pPion_->NotifyAttacking( *pTarget );
    pTarget->NotifyAttackedBy( *pPion_ );

    // Firers
    PHY_DirectFireData firerWeapons( *pPion_, nComposanteFiringType );
    GetRole< PHY_RolePion_Composantes >().FillDirectFireData( firerWeapons );
    if( !firerWeapons.HasWeaponsReady() )
    {
        if( firerWeapons.HasWeaponsNotReady() )
            return eRunning;
        if( firerWeapons.HasWeaponsAndNoAmmo() )
            return eNoAmmo;
        return eNoCapacity;
    }

    const uint nNbrWeaponsUsable = nFireMode == eModeNormal ? firerWeapons.GetNbrWeaponsUsable( rPercentageComposantesToUse ) : firerWeapons.GetNbrWeaponsUsable();

    // Targets
    PHY_RoleInterface_Composantes::T_ComposanteVector targets;
    pTarget->GetRole< PHY_RoleInterface_Composantes >().GetComposantesAbleToBeFired( targets, nNbrWeaponsUsable );
    if( targets.empty() )
        return eEnemyDestroyed;

    assert( targets.size() == nNbrWeaponsUsable );    
    Fire( firerWeapons, *pTarget, targets, nFireMode, *pFireResult );
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_DirectFiring::FireSuspended
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_RoleAction_DirectFiring::FireSuspended( uint nTargetKnowledgeID )
{
    MIL_Agent_ABC* pTarget = GetTarget( nTargetKnowledgeID );
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
void PHY_RoleAction_DirectFiring::FireZone( const MIL_ControlZone& zone, PHY_ControlZoneFireResults*& pFireResult )
{
    if( !pFireResult )
        pFireResult = new PHY_ControlZoneFireResults();

    assert( pPion_ );

    MIL_ControlZone::T_TargetVector targets;
    zone.GetTargets( *pPion_, targets );

    PHY_DirectFireData firerWeapons( *pPion_, PHY_DirectFireData::eFireComposantesLoadable );
    GetRole< PHY_RolePion_Composantes >().FillDirectFireData( firerWeapons );

    for( MIL_ControlZone::CIT_TargetVector itTarget = targets.begin(); itTarget != targets.end(); ++itTarget )
    {
        MIL_Agent_ABC&      target       = *itTarget->first;
        PHY_Composante_ABC& compTarget   = *itTarget->second;
        PHY_ComposantePion* pCompFirer   = 0;
        PHY_Weapon*         pFirerWeapon = 0;

        firerWeapons.ChooseRandomWeapon( target, compTarget, pCompFirer, pFirerWeapon );
        if( !pCompFirer )
            continue;

        assert( pFirerWeapon );
        pFirerWeapon->DirectFire( *pPion_, *itTarget->first, *itTarget->second, *pFireResult, false ); // 'false' is for 'don't use ph'
        firerWeapons.RemoveFirer( *pCompFirer );
    } 
}
