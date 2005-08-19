// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: PHY_RoleAction_IndirectFiring.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RoleAction_IndirectFiring.h"

#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "PHY_IndirectFireData.h"
#include "PHY_SmokeData.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_IndirectFiring, "PHY_RoleAction_IndirectFiring" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_IndirectFiring::PHY_RoleAction_IndirectFiring( MT_RoleContainer& role, MIL_AgentPion& pion )
    : MT_Role_ABC( role )
    , pPion_     ( &pion )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_IndirectFiring::PHY_RoleAction_IndirectFiring()
    : MT_Role_ABC()
    , pPion_     ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_IndirectFiring::~PHY_RoleAction_IndirectFiring()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_RoleAction_IndirectFiring::serialize( Archive& archive , const uint )
{
    archive & boost::serialization::base_object< MT_Role_ABC >( *this )
            & pPion_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::Fire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
int PHY_RoleAction_IndirectFiring::Fire( MIL_Effect_IndirectFire& effect )
{   
    if( effect.IsInterventionTypeFired() )
        return eFinished;

    if( !effect.IsTargetValid() )
        return eImpossible;

    // Firers
    assert( pPion_ );
    
    PHY_IndirectFireData firerWeapons( *pPion_, effect );
    GetRole< PHY_RolePion_Composantes >().FillIndirectFireData( firerWeapons );
    if( !firerWeapons.HasWeaponsReady() )
    {
        if( firerWeapons.HasWeaponsNotReady() )
            return eRunning;
        return eNoCapacity;
    }

    firerWeapons.PreselectWeapons();
    if( !effect.GetWeaponDotationCategory() )
        return eNoAmmo;

    // 
    PHY_ComposantePion* pFirer               = 0;
    PHY_Weapon*         pFirerWeapon         = 0;
    while( firerWeapons.GetUnusedFirerWeapon( pFirer, pFirerWeapon ) && !effect.IsInterventionTypeFired() )
    {
        if( !pFirerWeapon->IndirectFire( *pPion_, effect ) )
        {
            effect.ForceFlying();
            if( effect.GetNbrAmmoFired() )
                return eFinished;
            else
                return eNoAmmo;
        }
        firerWeapons.RemoveFirer( *pFirer );
    }

    if( effect.IsInterventionTypeFired() )
        return eFinished;

    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::FireSuspended
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_RoleAction_IndirectFiring::FireSuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::ThrowSmoke
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
int PHY_RoleAction_IndirectFiring::ThrowSmoke( const MT_Vector2D& vTargetPosition, uint nNbrAmmo )
{
    assert( pPion_ );
    
    PHY_SmokeData smokeData( *pPion_, PHY_IndirectFireDotationClass::fumigene_, nNbrAmmo );
    GetRole< PHY_RolePion_Composantes >().FillIndirectFireData( smokeData );
    
    PHY_Weapon* pWeapon = smokeData.GetWeapon();
    if( !pWeapon )
        return eNoCapacity;

    pWeapon->ThrowSmoke( *pPion_, pPion_->GetRole< PHY_RolePion_Location >().GetPosition(), vTargetPosition, nNbrAmmo );
    return eFinished;
}
