// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "DirectFireData.h"
#include "Weapon.h"
#include <wrapper/View.h>
#include <wrapper/Hook.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.hpp>
#include <algorithm>

using namespace sword;
using namespace sword::fire;

namespace
{
    DEFINE_HOOK( InitializeDecisional, 2, void, ( const char* xml, double tickDuration ) )
    {
        // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
        xml::xistringstream xis( xml );
        xis >> xml::start( "decisional" )
                >> xml::optional >> xml::start( "urban-combat" )
                    >> xml::attribute( "hit-factor", DirectFireData::nUrbanCoefficient_ )
                >> xml::end;
        if( GET_PREVIOUS_HOOK( InitializeDecisional ) )
            GET_PREVIOUS_HOOK( InitializeDecisional )( xml, tickDuration );
    }
}

DECLARE_HOOK( IsTemporarilyBlocked, bool, ( const SWORD_Model* entity, size_t nUrbanCoefficient ) )
DECLARE_HOOK( GetFireRandomInteger, size_t, ( size_t min, size_t max ) )

std::size_t DirectFireData::nUrbanCoefficient_ = 100;

// -----------------------------------------------------------------------------
// Name: DirectFireData constructor
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
DirectFireData::DirectFireData( ModuleFacade& module, const wrapper::View& firer, const wrapper::View& parameters )
    : module_              ( module )
    , firer_               ( firer )
    , parameters_          ( parameters )
    , bHasWeaponsReady_    ( true )
    , bHasWeaponsNotReady_ ( false )
    , bHasWeaponsAndNoAmmo_( false )
    , bTemporarilyBlocked_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::GetNbrWeaponsUsable
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
unsigned int DirectFireData::GetNbrWeaponsUsable() const
{
    if( parameters_[ "mode" ] == static_cast< int >( eFiringModeFree ) )
    {
        unsigned int nNbrWeaponsUsable = 0;
        for( CIT_ComposanteWeaponsMap itData = composantesWeapons_.begin(); itData != composantesWeapons_.end(); ++itData )
            nNbrWeaponsUsable += itData->second.GetNbrWeaponsUsable();
        return nNbrWeaponsUsable;
    }
    unsigned int nNbrUsedComposantes = 0;
    for( CIT_ComposanteWeaponsMap itData = composantesWeapons_.begin(); itData != composantesWeapons_.end(); ++itData )
        if( itData->second.IsFiring() )
            ++nNbrUsedComposantes;
    unsigned int nNbrComps = static_cast< unsigned int >( composantesWeapons_.size() * parameters_[ "percentage" ] );
    if( nNbrComps == 0 && ! composantesWeapons_.empty() )
        nNbrComps = 1;
    if( nNbrComps >= nNbrUsedComposantes )
        return nNbrComps - nNbrUsedComposantes;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::CanFire
// Created: LDC 2011-03-09
// -----------------------------------------------------------------------------
bool DirectFireData::CanFire( const wrapper::View& firer )
{
    bTemporarilyBlocked_ = GET_HOOK( ::IsTemporarilyBlocked )( firer, DirectFireData::nUrbanCoefficient_ );
    return ! bTemporarilyBlocked_;
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::ApplyOnWeapon
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
void DirectFireData::ApplyOnWeapon( const wrapper::View& model, const wrapper::View& component, const wrapper::View& weapon )
{
    const Weapon w( module_, model, weapon );
    if( ! w.CanDirectFire( component, parameters_ ) )
        return;
    if( ! w.HasDotation( firer_ ) )
        bHasWeaponsAndNoAmmo_ = true;
    else
    {
        sComposanteWeapons& data = composantesWeapons_[ component ];
        data.AddWeapon( w );
        bHasWeaponsNotReady_ |= data.IsFiring();
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::RemoveWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void DirectFireData::RemoveWeapon( const SWORD_Model* component, const Weapon& weapon )
{
    composantesWeapons_[ component ].RemoveWeapon( weapon );
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::RemoveFirer
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void DirectFireData::RemoveFirer( const SWORD_Model* component )
{
    composantesWeapons_.erase( component );
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::ChooseBestWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void DirectFireData::ChooseBestWeapon( const wrapper::View& target, const wrapper::View& compTarget, const SWORD_Model*& pBestFirer, const Weapon*& pBestWeapon ) const
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
// Name: DirectFireData::ChooseRandomWeapon
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
void DirectFireData::ChooseRandomWeapon( const wrapper::View& target, const wrapper::View& compTarget, const SWORD_Model*& pRandomFirer, const Weapon*& pRandomWeapon ) const
{
    const std::size_t nRnd = GET_HOOK( GetFireRandomInteger )( 0, composantesWeapons_.size() );
    CIT_ComposanteWeaponsMap it = composantesWeapons_.begin();
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
// Name: DirectFireData::GetUnusedFirerWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool DirectFireData::GetUnusedFirerWeapon( const SWORD_Model*& pUnusedFirer, const Weapon*& pUnusedFirerWeapon ) const
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
// Name: DirectFireData::ReleaseWeapon
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
void DirectFireData::ReleaseWeapon( const SWORD_Model* firer, const Weapon& weapon )
{
    if( parameters_[ "mode" ] == static_cast< int >( eFiringModeNormal ) )
        RemoveFirer( firer );
    else
        RemoveWeapon( firer, weapon );
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::HasWeaponsNotReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool DirectFireData::HasWeaponsNotReady() const
{
    return bHasWeaponsNotReady_;
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::HasWeaponsAndNoAmmo
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
bool DirectFireData::HasWeaponsAndNoAmmo() const
{
    return bHasWeaponsAndNoAmmo_;
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::IsTemporarilyBlocked
// Created: LDC 2011-05-16
// -----------------------------------------------------------------------------
bool DirectFireData::IsTemporarilyBlocked() const
{
    return bTemporarilyBlocked_;
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::sComposanteWeapons::sComposanteWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
DirectFireData::sComposanteWeapons::sComposanteWeapons()
    : bIsFiring_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::sComposanteWeapons::AddWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void DirectFireData::sComposanteWeapons::AddWeapon( const Weapon& weapon )
{
    if( weapon.IsReady() )
        weapons_.push_back( weapon );
    else
        bIsFiring_ = true;
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::sComposanteWeapons::RemoveWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void DirectFireData::sComposanteWeapons::RemoveWeapon( const Weapon& weapon )
{
    weapons_.erase( std::remove( weapons_.begin(), weapons_.end(), weapon ), weapons_.end() );
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::GetNbrWeaponsUsable
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
unsigned int DirectFireData::sComposanteWeapons::GetNbrWeaponsUsable() const
{
    return static_cast< unsigned >( weapons_.size() );
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::IsFiring
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool DirectFireData::sComposanteWeapons::IsFiring() const
{
    return bIsFiring_;
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::sComposanteWeapons::GetUnusedWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
const Weapon* DirectFireData::sComposanteWeapons::GetUnusedWeapon() const
{
    if( weapons_.empty() )
        return 0;
    return &weapons_.front();
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::sComposanteWeapons::GetBestWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool DirectFireData::sComposanteWeapons::GetBestWeapon( double& rBestScore, const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, const Weapon*& pBestWeapon ) const
{
    bool bUpdated = false;
    for( CIT_WeaponVector itWeapon = weapons_.begin(); itWeapon != weapons_.end(); ++itWeapon )
    {
        const Weapon& weapon = *itWeapon;
        double rCurrentScore = weapon.GetDangerosity( firer, target, compTarget, true, true ); // 'true' = 'use PH', true = "use ammo"
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
// Name: DirectFireData::sComposanteWeapons::GetRandomWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool DirectFireData::sComposanteWeapons::GetRandomWeapon( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, const Weapon*& pRandomWeapon ) const
{
    const std::size_t nRnd = GET_HOOK( GetFireRandomInteger )( 0, weapons_.size() );
    CIT_WeaponVector it = weapons_.begin();
    std::advance( it, nRnd );
    for( std::size_t i = 0; i < weapons_.size(); ++i )
    {
        const Weapon& weapon = *it;
        if( weapon.GetDangerosity( firer, target, compTarget, false, true ) > 0 ) // 'false' = 'don't use PH' 'true' = 'use ammo'
        {
            pRandomWeapon = &weapon;
            return true;
        }
        ++it;
        if( it == weapons_.end() )
            it = weapons_.begin();
    }
    return false;
}
