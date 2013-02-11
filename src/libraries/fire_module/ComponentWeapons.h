// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_ComponentWeapons_h
#define fire_module_ComponentWeapons_h

#include "Weapon.h"
#include <vector>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace fire
{
// =============================================================================
/** @class  ComponentWeapons
    @brief  Component weapons
*/
// Created: MCO 2013-02-08
// =============================================================================
class ComponentWeapons
{
public:
    //! @name Constructors/Destructor
    //@{
    ComponentWeapons();
    //@}

    //! @name Accessors
    //@{
    std::size_t GetUsableWeapons() const;
    const Weapon* GetUnusedWeapon() const;

    bool IsFiring() const;
    //@}

    //! @name Operations
    //@{
    void AddWeapon( const Weapon& weapon );
    void RemoveWeapon( const Weapon& weapon );

    bool GetBestWeapon( double& best, const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, const Weapon*& weapon ) const;
    //@}

private:
    //! @name Member data
    //@{
    bool firing_;
    std::vector< Weapon > weapons_;
    //@}
};

}
}

#endif // fire_module_ComponentWeapons_h
