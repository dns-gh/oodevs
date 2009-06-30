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

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_IndirectFiring.h"
#include "PHY_IndirectFireData.h"
#include "PHY_MunitionForIndirectFireData.h"
#include "PHY_SmokeData.h"
#include "Entities/Actions/PHY_FireResults_Default.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_IndirectFiring, "PHY_RoleAction_IndirectFiring" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_IndirectFiring::PHY_RoleAction_IndirectFiring( MT_RoleContainer& role, MIL_AgentPion& pion )
    : MT_Role_ABC( role )
    , pPion_     ( &pion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_IndirectFiring::PHY_RoleAction_IndirectFiring()
    : MT_Role_ABC()
    , pPion_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_IndirectFiring::~PHY_RoleAction_IndirectFiring()
{
    // NOTHING
}

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

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::Fire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
int PHY_RoleAction_IndirectFiring::Fire( MIL_Effect_IndirectFire* pEffect )
{   
    if( !pEffect )
        return eImpossible;

    if( pEffect->IsInterventionTypeFired() )
        return eFinished;

    if( !pEffect->IsTargetValid() ) 
        return eImpossible;

    // Firers
    assert( pPion_ );
    
    PHY_IndirectFireData firerWeapons( *pPion_, *pEffect );
    GetRole< PHY_RolePion_Composantes >().ApplyOnWeapons( firerWeapons );
    if( !firerWeapons.HasWeaponsReady() )
    {
        if( firerWeapons.HasWeaponsNotReady() )
            return eRunning;
        if( firerWeapons.HasWeaponsAndNoAmmo() )
            return eNoAmmo;
        return eNoCapacity;
    }

    // 
    const PHY_ComposantePion* pFirer         = 0;
    PHY_Weapon*         pFirerWeapon         = 0;
    while( firerWeapons.GetUnusedFirerWeapon( pFirer, pFirerWeapon ) && !pEffect->IsInterventionTypeFired() ) // ready weapons
    {
        if( !pFirerWeapon->IndirectFire( *pPion_, *pEffect ) )
        {
            pEffect->ForceFlying(); 
            if( pEffect->GetNbrAmmoFired() )
                return eFinished;
            else
                return eNoAmmo;
        }
        firerWeapons.RemoveFirer( *pFirer );
    }

    if( pEffect->IsInterventionTypeFired() )
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
    GetRole< PHY_RolePion_Composantes >().ApplyOnWeapons( smokeData );
    
    PHY_Weapon* pWeapon = smokeData.GetWeapon();
    if( !pWeapon )
        return eNoCapacity;

    PHY_FireResults_Default fireResult; //$$$ POURRI
    pWeapon->ThrowSmoke( *pPion_, pPion_->GetRole< PHY_RolePion_Location >().GetPosition(), vTargetPosition, nNbrAmmo, fireResult );
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::GetMunitionForIndirectFire
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_RoleAction_IndirectFiring::GetMunitionForIndirectFire( const PHY_IndirectFireDotationClass& indirectWeaponCategory, const MT_Vector2D& vTargetPosition )
{
    assert( pPion_ );

    const MT_Float rRange = pPion_->GetRole< PHY_RolePion_Location >().GetPosition().Distance( vTargetPosition );
    
    PHY_MunitionForIndirectFireData fireData( *pPion_, indirectWeaponCategory, rRange );
    GetRole< PHY_RolePion_Composantes >().ApplyOnWeapons( fireData );

    return fireData.GetChoosenMunition();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::GetInitialReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
int PHY_RoleAction_IndirectFiring::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::GetFinalReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
int PHY_RoleAction_IndirectFiring::GetFinalReturnCode() const
{
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::Update
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_RoleAction_IndirectFiring::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::Clean
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_RoleAction_IndirectFiring::Clean()
{
    // NOTHING
}
