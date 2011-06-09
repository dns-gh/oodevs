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
#include "AlgorithmsFactories.h"
#include "WeaponAvailabilityComputer_ABC.h"
#include "WeaponAvailabilityComputerFactory_ABC.h"
#include "PHY_IndirectFireData.h"
#include "PHY_MunitionForIndirectFireData.h"
#include "PHY_SmokeData.h"
#include "Entities/Actions/PHY_FireResults_Default.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"

using namespace firing;

BOOST_CLASS_EXPORT_IMPLEMENT( firing::PHY_RoleAction_IndirectFiring )

using namespace firing;

template< typename Archive >
void firing::save_construct_data( Archive& archive, const PHY_RoleAction_IndirectFiring* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void firing::load_construct_data( Archive& archive, PHY_RoleAction_IndirectFiring* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RoleAction_IndirectFiring( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_RoleAction_IndirectFiring::PHY_RoleAction_IndirectFiring( MIL_Agent_ABC& pion )
    : pion_( pion )
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
void PHY_RoleAction_IndirectFiring::serialize( Archive& archive , const unsigned int )
{
    archive & boost::serialization::base_object< tools::Role_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::Fire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
int PHY_RoleAction_IndirectFiring::Fire( MIL_Effect_IndirectFire* pEffect )
{
    if( !pEffect )
        return eImpossible;

    if( pEffect->MustWaitImpact() )
        return eRunning;
    if( pEffect->IsInterventionTypeFired() )
        return eFinished;

    if( !pEffect->IsTargetValid() )
        return eImpossible;

    // Firers
    PHY_IndirectFireData firerWeapons( pion_, *pEffect );
    std::auto_ptr< WeaponAvailabilityComputer_ABC > weaponAvailabilityComputer( pion_.GetAlgorithms().weaponAvailabilityComputerFactory_->Create( firerWeapons ) );
    pion_.Execute( *weaponAvailabilityComputer );

    if( !firerWeapons.HasWeaponsReady() )
    {
        if( firerWeapons.HasWeaponsNotReady() )
            return eRunning;
        if( firerWeapons.HasWeaponsAndNoAmmo() )
            return eNoAmmo;
        return eNoCapacity;
    }

    //
    const PHY_ComposantePion* pFirer = 0;
    PHY_Weapon* pFirerWeapon = 0;
    while( firerWeapons.GetUnusedFirerWeapon( pFirer, pFirerWeapon ) && !pEffect->IsInterventionTypeFired() ) // ready weapons
    {
        if( !pFirerWeapon->IndirectFire( pion_, *pEffect ) )
        {
            pEffect->ForceFlying();
            if( pEffect->GetNbrAmmoFired() )
                return eFinished;
            else
                return eNoAmmo;
        }
        firerWeapons.RemoveFirer( *pFirer );
    }

    if( pEffect->IsInterventionTypeFired() && !pEffect->MustWaitImpact() )
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
int PHY_RoleAction_IndirectFiring::ThrowSmoke( const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo )
{
    PHY_SmokeData smokeData( pion_, PHY_IndirectFireDotationClass::fumigene_, nNbrAmmo );
    std::auto_ptr< WeaponAvailabilityComputer_ABC > weaponAvailabilityComputer( pion_.GetAlgorithms().weaponAvailabilityComputerFactory_->Create( smokeData ) );
    pion_.Execute( *weaponAvailabilityComputer );

    PHY_Weapon* pWeapon = smokeData.GetWeapon();
    if( !pWeapon )
        return eNoCapacity;

    PHY_FireResults_Default fireResult; //$$$ POURRI
    pWeapon->ThrowSmoke( pion_, vTargetPosition, nNbrAmmo, fireResult );
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_IndirectFiring::GetMunitionForIndirectFire
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_RoleAction_IndirectFiring::GetMunitionForIndirectFire( const PHY_IndirectFireDotationClass& indirectWeaponCategory, const MT_Vector2D& vTargetPosition )
{
    PHY_MunitionForIndirectFireData fireData( pion_, indirectWeaponCategory, vTargetPosition );
    std::auto_ptr< WeaponAvailabilityComputer_ABC > weaponAvailabilityComputer( pion_.GetAlgorithms().weaponAvailabilityComputerFactory_->Create( fireData ) );
    pion_.Execute( *weaponAvailabilityComputer );

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
