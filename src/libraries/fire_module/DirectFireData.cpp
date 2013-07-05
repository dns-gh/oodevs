// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "DirectFireData.h"
#include <wrapper/View.h>
#include <wrapper/Hook.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.hpp>
#include <boost/range/algorithm/count_if.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/numeric.hpp>

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
    , entity_               ( firer )
    , parameters_          ( parameters )
    , bHasWeaponsReady_    ( true )
    , bHasWeaponsNotReady_ ( false )
    , bHasWeaponsAndNoAmmo_( false )
    , bTemporarilyBlocked_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::GetUsableWeapons
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
std::size_t DirectFireData::GetUsableWeapons() const
{
    if( weapons_.empty() )
        return 0;
    const auto weapons = weapons_ | boost::adaptors::map_values;
    if( parameters_[ "mode" ] == static_cast< int >( eFiringModeFree ) )
        return boost::accumulate( weapons | boost::adaptors::transformed( boost::mem_fn( &ComponentWeapons::GetUsableWeapons ) ), 0 );
    const double percentage = parameters_[ "percentage" ];
    if( percentage < 0 || percentage > 1 )
        throw std::invalid_argument( "percentage parameter must be >= 0 and <= 1" );
    const std::size_t wanted = static_cast< std::size_t >( std::max( 1., weapons_.size() * percentage ) );
    const std::size_t used = boost::count_if( weapons, boost::mem_fn( &ComponentWeapons::IsFiring ) );
    if( wanted >= used )
        return wanted - used;
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
    if( ! w.HasDotation( entity_ ) )
        bHasWeaponsAndNoAmmo_ = true;
    else
    {
        ComponentWeapons& data = weapons_[ component ];
        data.AddWeapon( w );
        bHasWeaponsNotReady_ |= data.IsFiring();
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::Fire
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void DirectFireData::Fire( const wrapper::View& target, const T_Components& targets )
{
    assert( GetUsableWeapons() == targets.size() );
    for( auto it = targets.begin(); it != targets.end(); ++it )
        FireBestWeapon( target, *it );
    FireRemainingWeapons( target, targets );
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::Fire
// Created: MCO 2013-02-08
// -----------------------------------------------------------------------------
void DirectFireData::Fire( const wrapper::View& element )
{
    const SWORD_Model* firer = 0;
    const Weapon* weapon = 0;
    while( GetUnusedFirerWeapon( firer, weapon ) )
    {
        weapon->DirectFire( entity_, element );
        ReleaseWeapon( firer, *weapon );
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::FireRemainingWeapons
// Created: MCO 2013-02-08
// -----------------------------------------------------------------------------
void DirectFireData::FireRemainingWeapons( const wrapper::View& target, const T_Components& targets )
{
    const SWORD_Model* firer = 0;
    const Weapon* weapon = 0;
    while( GetUnusedFirerWeapon( firer, weapon ) )
    {
        const wrapper::View* pBestCompTarget = 0;
        double bestScore = 0;
        for( auto it = targets.begin(); it != targets.end(); ++it )
        {
            const double score = weapon->GetDangerosity( entity_, target, *it, true, true ); // 'true' is for 'use ph' and true for 'use ammo'
            if( score > bestScore )
            {
                bestScore = score;
                pBestCompTarget = &*it;
            }
        }
        if( pBestCompTarget )
            weapon->DirectFire( entity_, target, *pBestCompTarget, true ); // 'true' is for 'use ph'
        ReleaseWeapon( firer, *weapon );
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::FireBestWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void DirectFireData::FireBestWeapon( const wrapper::View& target, const wrapper::View& compTarget )
{
    const SWORD_Model* firer = 0;
    const Weapon* weapon = 0;
    double score = 0;
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
        if( it->second.GetBestWeapon( score, entity_, target, compTarget, weapon ) )
            firer = it->first;
    if( firer )
    {
        weapon->DirectFire( entity_, target, compTarget, true ); // 'true' is for 'use ph'
        ReleaseWeapon( firer, *weapon );
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::GetUnusedFirerWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool DirectFireData::GetUnusedFirerWeapon( const SWORD_Model*& firer, const Weapon*& weapon ) const
{
    weapon = 0;
    firer = 0;
    if( weapons_.empty() )
        return false;
    auto it = weapons_.begin();
    weapon = it->second.GetUnusedWeapon();
    if( !weapon )
        return false;
    firer = it->first;
    return true;
}

// -----------------------------------------------------------------------------
// Name: DirectFireData::ReleaseWeapon
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
void DirectFireData::ReleaseWeapon( const SWORD_Model* firer, const Weapon& weapon )
{
    if( parameters_[ "mode" ] == static_cast< int >( eFiringModeNormal ) )
        weapons_.erase( firer );
    else
        weapons_[ firer ].RemoveWeapon( weapon );
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
