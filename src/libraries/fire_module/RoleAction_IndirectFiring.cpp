// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "RoleAction_IndirectFiring.h"
#include "AmmunitionForIndirectFireData.h"

using namespace sword;
using namespace sword::fire;

//// -----------------------------------------------------------------------------
//// Name: RoleAction_IndirectFiring::Fire
//// Created: NLD 2004-10-11
//// -----------------------------------------------------------------------------
//int RoleAction_IndirectFiring::Fire( MIL_Effect_IndirectFire* pEffect )
//{
//    if( !pEffect )
//        return eImpossible;
//
//    if( pEffect->MustWaitImpact() )
//        return eRunning;
//    if( pEffect->IsInterventionTypeFired() )
//        return eFinished;
//
//    if( !pEffect->IsTargetValid() )
//        return eImpossible;
//
//    // Firers
//    PHY_IndirectFireData firerWeapons( owner_, *pEffect );
//    std::auto_ptr< WeaponAvailabilityComputer_ABC > weaponAvailabilityComputer( owner_.GetAlgorithms().weaponAvailabilityComputerFactory_->Create( firerWeapons ) );
//    owner_.Execute( *weaponAvailabilityComputer );
//
//    if( !firerWeapons.HasWeaponsReady() )
//    {
//        if( firerWeapons.HasWeaponsNotReady() )
//            return eRunning;
//        if( firerWeapons.HasWeaponsAndNoAmmo() )
//            return eNoAmmo;
//        return eNoCapacity;
//    }
//
//    //
//    const PHY_ComposantePion* pFirer = 0;
//    PHY_Weapon* pFirerWeapon = 0;
//    while( firerWeapons.GetUnusedFirerWeapon( pFirer, pFirerWeapon ) && !pEffect->IsInterventionTypeFired() ) // ready weapons
//    {
//        if( !pFirerWeapon->IndirectFire( owner_, *pEffect ) )
//        {
//            pEffect->ForceFlying();
//            if( pEffect->GetNbrAmmoFired() )
//                return eFinished;
//            else
//                return eNoAmmo;
//        }
//        firerWeapons.RemoveFirer( *pFirer );
//    }
//
//    if( pEffect->IsInterventionTypeFired() && !pEffect->MustWaitImpact() )
//        return eFinished;
//
//    return eRunning;
//}
//
//// -----------------------------------------------------------------------------
//// Name: RoleAction_IndirectFiring::FireSuspended
//// Created: NLD 2004-10-06
//// -----------------------------------------------------------------------------
//void RoleAction_IndirectFiring::FireSuspended()
//{
//    // NOTHING
//}
//
//// -----------------------------------------------------------------------------
//// Name: RoleAction_IndirectFiring::ThrowSmoke
//// Created: NLD 2004-10-21
//// -----------------------------------------------------------------------------
//int RoleAction_IndirectFiring::ThrowSmoke( const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo )
//{
//    PHY_SmokeData smokeData( owner_, PHY_IndirectFireDotationClass::fumigene_, nNbrAmmo );
//    std::auto_ptr< WeaponAvailabilityComputer_ABC > weaponAvailabilityComputer( owner_.GetAlgorithms().weaponAvailabilityComputerFactory_->Create( smokeData ) );
//    owner_.Execute( *weaponAvailabilityComputer );
//
//    PHY_Weapon* pWeapon = smokeData.GetWeapon();
//    if( !pWeapon )
//        return eNoCapacity;
//
//    PHY_FireResults_Default fireResult; //$$$ POURRI
//    pWeapon->ThrowSmoke( owner_, vTargetPosition, nNbrAmmo, fireResult );
//    return eFinished;
//}

// -----------------------------------------------------------------------------
// Name: RoleAction_IndirectFiring::GetAmmunitionForIndirectFire
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
int RoleAction_IndirectFiring::GetAmmunitionForIndirectFire( ModuleFacade& module, const wrapper::View& model, const wrapper::View& firer, const std::string& type, const MT_Vector2D* target )
{
    AmmunitionForIndirectFireData data( firer, type, target );
    const wrapper::View& components = firer[ "components" ];
    for( std::size_t c = 0; c < components.GetSize(); ++c )
    {
        const wrapper::View& component = components.GetElement( c );
        const wrapper::View& weapons = component[ "weapons" ];
        for( std::size_t w = 0; w < weapons.GetSize(); ++w )
            data.ApplyOnWeapon( module, model, component, weapons.GetElement( w ) );
    }
    return data.GetResult();
}

//// -----------------------------------------------------------------------------
//// Name: RoleAction_IndirectFiring::GetInitialReturnCode
//// Created: NLD 2004-10-06
//// -----------------------------------------------------------------------------
//int RoleAction_IndirectFiring::GetInitialReturnCode() const
//{
//    return eRunning;
//}
//
//// -----------------------------------------------------------------------------
//// Name: RoleAction_IndirectFiring::GetFinalReturnCode
//// Created: NLD 2004-10-06
//// -----------------------------------------------------------------------------
//int RoleAction_IndirectFiring::GetFinalReturnCode() const
//{
//    return eFinished;
//}
//
//// -----------------------------------------------------------------------------
//// Name: RoleAction_IndirectFiring::Update
//// Created: NLD 2004-10-06
//// -----------------------------------------------------------------------------
//void RoleAction_IndirectFiring::Update( bool /*bIsDead*/ )
//{
//    // NOTHING
//}
//
//// -----------------------------------------------------------------------------
//// Name: RoleAction_IndirectFiring::Clean
//// Created: NLD 2004-10-06
//// -----------------------------------------------------------------------------
//void RoleAction_IndirectFiring::Clean()
//{
//    // NOTHING
//}
