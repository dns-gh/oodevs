// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_DirectFireData.cpp $
// $Author: Nld $
// $Modtime: 18/04/05 16:58 $
// $Revision: 5 $
// $Workfile: PHY_DirectFireData.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DirectFireData.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "MIL_Random.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/DotationComputer_ABC.h"
#include "simulation_kernel/DotationComputerFactory_ABC.h"
#include "MT_Tools/MT_Logger.h"

using namespace firing;

unsigned int PHY_DirectFireData::nUrbanCoefficient_ = 100;

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::sComposanteWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_DirectFireData::sComposanteWeapons::sComposanteWeapons()
    : bIsFiring_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::~sComposanteWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_DirectFireData::sComposanteWeapons::~sComposanteWeapons()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::AddWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_DirectFireData::sComposanteWeapons::AddWeapon( PHY_Weapon& weapon )
{
    if( weapon.IsReady() )
        weaponsReady_.push_back( &weapon );
    else
        bIsFiring_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::RemoveWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_DirectFireData::sComposanteWeapons::RemoveWeapon( PHY_Weapon& weapon )
{
    weaponsReady_.erase( std::find( weaponsReady_.begin(), weaponsReady_.end(), &weapon ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData constructor
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_DirectFireData::PHY_DirectFireData( MIL_AgentPion& firer, E_ComposanteFiringType nComposanteFiringType, E_FiringMode nFiringMode, double rPercentageComposantesToUse, const PHY_AmmoDotationClass* pAmmoDotationClass /* = 0 */ )
    : firer_                      ( firer )
    , nComposanteFiringType_      ( nComposanteFiringType )
    , nFiringMode_                ( nFiringMode )
    , rPercentageComposantesToUse_( rPercentageComposantesToUse )
    , pAmmoDotationClass_         ( pAmmoDotationClass )
    , bHasWeaponsReady_           ( true )
    , bHasWeaponsNotReady_        ( false )
    , bHasWeaponsAndNoAmmo_       ( false )
    , bTemporarilyBlocked_        ( false )
{
    assert( rPercentageComposantesToUse_  >= 0. && rPercentageComposantesToUse_ <= 1. );
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData destructor
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_DirectFireData::~PHY_DirectFireData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::GetNbrWeaponsUsable
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
unsigned int PHY_DirectFireData::GetNbrWeaponsUsable() const
{
    if( nFiringMode_ == eFiringModeFree )
    {
        unsigned int nNbrWeaponsUsable = 0;
        for( auto itData = composantesWeapons_.begin(); itData != composantesWeapons_.end(); ++itData )
            nNbrWeaponsUsable += itData->second.GetNbrWeaponsUsable();
        return nNbrWeaponsUsable;
    }
    else if( nFiringMode_ == eFiringModeNormal )
    {
        unsigned int nNbrUsedComposantes = 0;
        for( auto itData = composantesWeapons_.begin(); itData != composantesWeapons_.end(); ++itData )
            if( itData->second.IsFiring() )
                ++ nNbrUsedComposantes;

        unsigned int nNbrComps = (unsigned int)( composantesWeapons_.size() * rPercentageComposantesToUse_ );
        if( !( nNbrComps || composantesWeapons_.empty() ) )
            nNbrComps = 1;

        if( nNbrComps >= nNbrUsedComposantes )
            return nNbrComps - nNbrUsedComposantes;
        return 0;
    }

    assert( false );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::operator()
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
void PHY_DirectFireData::operator() ( const PHY_ComposantePion& compFirer, PHY_Weapon& weapon )
{
    if( ! CanFire() )
        return;

    if( !compFirer.CanFire() || !weapon.CanDirectFire() )
        return;

    if( nComposanteFiringType_ == eFireUsingOnlyComposantesLoadable && !compFirer.CanBeLoaded() )
        return;

    if( nComposanteFiringType_ == eFireUsingOnlyComposantesCarrier && !compFirer.CanTransportHumans() )
        return;

    if( pAmmoDotationClass_ && ( !weapon.GetDotationCategory().GetAmmoDotationClass() || *weapon.GetDotationCategory().GetAmmoDotationClass() != *pAmmoDotationClass_ ) )
        return;

    std::auto_ptr< dotation::DotationComputer_ABC > dotationComputer( firer_.GetAlgorithms().dotationComputerFactory_->Create() );
    firer_.Execute( *dotationComputer );
    if( !dotationComputer->HasDotation( weapon.GetDotationCategory() ) )
        bHasWeaponsAndNoAmmo_ = true;
    else
    {
        sComposanteWeapons& data = composantesWeapons_[ &compFirer ];
        data.AddWeapon( weapon );

        bHasWeaponsNotReady_ |= data.IsFiring();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::CanFire
// Created: LDC 2011-03-09
// -----------------------------------------------------------------------------
bool PHY_DirectFireData::CanFire()
{
    if( !firer_.GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity() )
        return true;
    bTemporarilyBlocked_ = !( MT_Random::GetInstance().rand32_io( 0u, 100u ) < PHY_DirectFireData::nUrbanCoefficient_ );
    return !bTemporarilyBlocked_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::RemoveWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_DirectFireData::RemoveWeapon( const PHY_ComposantePion& firer, PHY_Weapon& weapon )
{
    sComposanteWeapons& data = composantesWeapons_[ &firer ];
    data.RemoveWeapon( weapon );
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::RemoveFirer
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_DirectFireData::RemoveFirer( const PHY_ComposantePion& firer )
{
    if( composantesWeapons_.erase( &firer ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::GetBestWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool PHY_DirectFireData::sComposanteWeapons::GetBestWeapon( double& rBestScore, const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, PHY_Weapon*& pBestWeapon ) const
{
    bool bUpdated = false;
    for( CIT_WeaponVector itWeapon = weaponsReady_.begin(); itWeapon != weaponsReady_.end(); ++itWeapon )
    {
        PHY_Weapon& weapon = **itWeapon;
        double rCurrentScore = weapon.GetDangerosity( firer, target, compTarget.GetType(), true, true ); // 'true' = 'use PH', true = "use ammo"
        if( rCurrentScore >= rBestScore )
        {
            bUpdated = true;
            rBestScore  = rCurrentScore;
            pBestWeapon = &weapon;
        }
    }
    return bUpdated;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::GetRandomWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool PHY_DirectFireData::sComposanteWeapons::GetRandomWeapon( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, PHY_Weapon*& pRandomWeapon ) const
{
    const std::size_t nRnd = MIL_Random::rand32_ii( 0, static_cast< unsigned long >( weaponsReady_.size() - 1 ) );

    CIT_WeaponVector it = weaponsReady_.begin();
    std::advance( it, nRnd );

    for( std::size_t i = 0; i < weaponsReady_.size(); ++i )
    {
        PHY_Weapon& weapon = **it;
        if( weapon.GetDangerosity( firer, target, compTarget.GetType(), false, true ) > 0 ) // 'false' = 'don't use PH' 'true' = 'use ammo'
        {
            pRandomWeapon = &weapon;
            return true;
        }

        ++it;
        if( it == weaponsReady_.end() )
            it = weaponsReady_.begin();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::ChooseBestWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_DirectFireData::ChooseBestWeapon( const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, const PHY_ComposantePion*& pBestFirer, PHY_Weapon*& pBestWeapon ) const
{
    double rBestScore = 0;
    pBestFirer  = 0;
    pBestWeapon = 0;
    for( auto it = composantesWeapons_.begin(); it != composantesWeapons_.end(); ++it )
    {
        const sComposanteWeapons& data = it->second;

        bool bUpdated = data.GetBestWeapon( rBestScore, firer_, target, compTarget, pBestWeapon );
        if( bUpdated )
            pBestFirer = it->first;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::ChooseRandomWeapon
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
void PHY_DirectFireData::ChooseRandomWeapon( const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, const PHY_ComposantePion*& pRandomFirer, PHY_Weapon*& pRandomWeapon ) const
{
    const std::size_t nRnd = MIL_Random::rand32_ii( 0, static_cast< unsigned long >( composantesWeapons_.size() - 1 ) );

    auto it = composantesWeapons_.begin();
    std::advance( it, nRnd );

    for( std::size_t i = 0; i < composantesWeapons_.size(); ++i )
    {
        const sComposanteWeapons& data = it->second;

        if( data.GetRandomWeapon( firer_, target, compTarget, pRandomWeapon ) )
        {
            pRandomFirer = it->first;
            return;
        }

        ++it;
        if( it == composantesWeapons_.end() )
            it = composantesWeapons_.begin();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::GetUnusedFirerWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool PHY_DirectFireData::GetUnusedFirerWeapon( const PHY_ComposantePion*& pUnusedFirer, PHY_Weapon*& pUnusedFirerWeapon ) const
{
    pUnusedFirerWeapon = 0;
    pUnusedFirer       = 0;
    if( composantesWeapons_.empty() )
        return false;

    auto it = composantesWeapons_.begin();
    pUnusedFirerWeapon = it->second.GetUnusedWeapon();
    if( !pUnusedFirerWeapon )
        return false;
    pUnusedFirer = it->first;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::ReleaseWeapon
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
void PHY_DirectFireData::ReleaseWeapon( const PHY_ComposantePion& firer, PHY_Weapon& weapon )
{
    switch( nFiringMode_ )
    {
        case eFiringModeNormal : RemoveFirer ( firer ); break;
        case eFiringModeFree   : RemoveWeapon( firer, weapon ); break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::HasWeaponsNotReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool PHY_DirectFireData::HasWeaponsNotReady() const
{
    return bHasWeaponsNotReady_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::HasWeaponsAndNoAmmo
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
bool PHY_DirectFireData::HasWeaponsAndNoAmmo() const
{
    return bHasWeaponsAndNoAmmo_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::IsTemporarilyBlocked
// Created: LDC 2011-05-16
// -----------------------------------------------------------------------------
bool PHY_DirectFireData::IsTemporarilyBlocked() const
{
    return bTemporarilyBlocked_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::GetNbrWeaponsUsable
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
unsigned int PHY_DirectFireData::sComposanteWeapons::GetNbrWeaponsUsable() const
{
    return static_cast< unsigned >( weaponsReady_.size() );
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::IsFiring
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool PHY_DirectFireData::sComposanteWeapons::IsFiring() const
{
    return bIsFiring_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::GetUnusedWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_Weapon* PHY_DirectFireData::sComposanteWeapons::GetUnusedWeapon() const
{
    if( weaponsReady_.empty() )
        return 0;
    return weaponsReady_.front();
}
