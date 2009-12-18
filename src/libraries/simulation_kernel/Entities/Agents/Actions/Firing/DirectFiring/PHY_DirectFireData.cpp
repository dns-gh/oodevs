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

#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/DotationComputer_ABC.h"
#include "simulation_kernel/DotationComputerFactory_ABC.h"

using namespace firing;

MT_Random PHY_DirectFireData::randomGenerator_;

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::sComposanteWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_DirectFireData::sComposanteWeapons::sComposanteWeapons()
    : bIsFiring_   ( false )
    , weaponsReady_()
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
PHY_DirectFireData::PHY_DirectFireData( MIL_AgentPion& firer, E_ComposanteFiringType nComposanteFiringType, E_FiringMode nFiringMode, MT_Float rPercentageComposantesToUse, const PHY_AmmoDotationClass* pAmmoDotationClass /* = 0 */ )
    : bHasWeaponsNotReady_        ( false )
    , bHasWeaponsAndNoAmmo_       ( false )
    , firer_                      ( firer )
    , nComposanteFiringType_      ( nComposanteFiringType )
    , nFiringMode_                ( nFiringMode )
    , rPercentageComposantesToUse_( rPercentageComposantesToUse )
    , pAmmoDotationClass_         ( pAmmoDotationClass )
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
uint PHY_DirectFireData::GetNbrWeaponsUsable() const
{
    if( nFiringMode_ == eFiringModeFree )
    {
        uint nNbrWeaponsUsable = 0;
        for( CIT_ComposanteWeaponsMap itData = composantesWeapons_.begin(); itData != composantesWeapons_.end(); ++itData )
            nNbrWeaponsUsable += itData->second.GetNbrWeaponsUsable();
        return nNbrWeaponsUsable;
    }
    else if( nFiringMode_ == eFiringModeNormal )
    {
        uint nNbrUsedComposantes = 0;
        for( CIT_ComposanteWeaponsMap itData = composantesWeapons_.begin(); itData != composantesWeapons_.end(); ++itData )
            if( itData->second.IsFiring() )
                ++ nNbrUsedComposantes;
            
        uint nNbrComps = (uint)( composantesWeapons_.size() * rPercentageComposantesToUse_ );
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
    int nOut = composantesWeapons_.erase( &firer );
    assert( nOut == 1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_DirectFireData::sComposanteWeapons::GetBestWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool PHY_DirectFireData::sComposanteWeapons::GetBestWeapon( MT_Float& rBestScore, const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, PHY_Weapon*& pBestWeapon ) const
{
    bool bUpdated = false;
    for( CIT_WeaponVector itWeapon = weaponsReady_.begin(); itWeapon != weaponsReady_.end(); ++itWeapon )
    {
        PHY_Weapon& weapon = **itWeapon;
        MT_Float rCurrentScore = weapon.GetDangerosity( firer, target, compTarget.GetType(), true ); // 'true' = 'use PH'
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
    const uint nRnd = randomGenerator_.rand32_ii( 0, weaponsReady_.size() - 1 );

    CIT_WeaponVector it = weaponsReady_.begin();
    std::advance( it, nRnd );

    for( uint i = 0; i < weaponsReady_.size(); ++i )
    {
        PHY_Weapon& weapon = **it;
        if( weapon.GetDangerosity( firer, target, compTarget.GetType(), false  ) > 0 ) // 'true' = 'don't use PH'
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
    MT_Float rBestScore = 0;
    pBestFirer  = 0;
    pBestWeapon = 0;
    for( CIT_ComposanteWeaponsMap it = composantesWeapons_.begin(); it != composantesWeapons_.end(); ++it )
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
    const uint nRnd = randomGenerator_.rand32_ii( 0, composantesWeapons_.size() - 1 );

    CIT_ComposanteWeaponsMap it = composantesWeapons_.begin();
    std::advance( it, nRnd );

    for( uint i = 0; i < composantesWeapons_.size(); ++i )
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

    CIT_ComposanteWeaponsMap it = composantesWeapons_.begin();
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
// Name: PHY_DirectFireData::GetNbrWeaponsUsable
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
uint PHY_DirectFireData::sComposanteWeapons::GetNbrWeaponsUsable() const
{
    return weaponsReady_.size();
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
