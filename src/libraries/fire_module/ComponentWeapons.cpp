// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ComponentWeapons.h"
#include <wrapper/Hook.h>
#include <boost/range/algorithm_ext/erase.hpp>

using namespace sword;
using namespace sword::fire;

DECLARE_HOOK( GetFireRandomInteger, size_t, ( size_t min, size_t max ) )

// -----------------------------------------------------------------------------
// Name: ComponentWeapons::ComponentWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
ComponentWeapons::ComponentWeapons()
    : firing_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ComponentWeapons::AddWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void ComponentWeapons::AddWeapon( const Weapon& weapon )
{
    if( weapon.IsReady() )
        weapons_.push_back( weapon );
    else
        firing_ = true;
}

// -----------------------------------------------------------------------------
// Name: ComponentWeapons::RemoveWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void ComponentWeapons::RemoveWeapon( const Weapon& weapon )
{
    boost::remove_erase( weapons_, weapon );
}

// -----------------------------------------------------------------------------
// Name: GetUsableWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
std::size_t ComponentWeapons::GetUsableWeapons() const
{
    return weapons_.size();
}

// -----------------------------------------------------------------------------
// Name: IsFiring
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool ComponentWeapons::IsFiring() const
{
    return firing_;
}

// -----------------------------------------------------------------------------
// Name: ComponentWeapons::GetUnusedWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
const Weapon* ComponentWeapons::GetUnusedWeapon() const
{
    if( weapons_.empty() )
        return 0;
    return &weapons_.front();
}

// -----------------------------------------------------------------------------
// Name: ComponentWeapons::GetBestWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool ComponentWeapons::GetBestWeapon( double& best, const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, const Weapon*& weapon ) const
{
    bool updated = false;
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        const double score = it->GetDangerosity( firer, target, compTarget, true, true ); // 'true' = 'use PH', true = "use ammo"
        if( score >= best )
        {
            updated = true;
            best = score;
            weapon = &(*it);
        }
    }
    return updated;
}
