// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_DirectFireData_h
#define fire_module_DirectFireData_h

#include "ComponentWeapons.h"
#include "tools/Map.h"
#include <boost/noncopyable.hpp>
#include <vector>
#include <map>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace fire
{
    class ModuleFacade;

// =============================================================================
/** @class  DirectFireData
    @brief  Direct fire data
*/
// Created: JVT 2004-08-03
// =============================================================================
class DirectFireData : boost::noncopyable
{
private:
    typedef std::vector< wrapper::View > T_Components;

public:
    //! @name Types
    //@{
    enum E_FiringMode
    {
        eFiringModeNormal, // Chaque composante fait feu avec le meilleur couple ( lanceur / munition )
        eFiringModeFree    // Chaque composante fait feu avec tous les lanceurs
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    DirectFireData( ModuleFacade& module, const wrapper::View& firer, const wrapper::View& parameters );
    //@}

    //! @name Operations
    //@{
    void ApplyOnWeapon( const wrapper::View& model, const wrapper::View& component, const wrapper::View& weapon );

    void Fire( const wrapper::View& target, const T_Components& compTargets );
    void Fire( const wrapper::View& element );

    bool CanFire( const wrapper::View& firer );
    //@}

    //! @name Accessors
    //@{
    bool HasWeaponsNotReady() const;
    bool HasWeaponsAndNoAmmo() const;
    bool IsTemporarilyBlocked() const;
    std::size_t GetUsableWeapons() const;
    //@}

private:
    void FireBestWeapon( const wrapper::View& target, const wrapper::View& compTarget );
    void FireRemainingWeapons( const wrapper::View& target, const T_Components& targets );
    bool GetUnusedFirerWeapon( const SWORD_Model*& firer, const Weapon*& weapon ) const;
    void ReleaseWeapon( const SWORD_Model* firer, const Weapon& weapon );

private:
    //! @name Member data
    //@{
    ModuleFacade& module_;
    const wrapper::View entity_;
    const wrapper::View parameters_;
    bool bHasWeaponsReady_;
    bool bHasWeaponsNotReady_;
    bool bHasWeaponsAndNoAmmo_;
    bool bTemporarilyBlocked_;

    tools::Map< const SWORD_Model*, ComponentWeapons > weapons_;
    //@}

public:
    // Factor applied to number of components that can fire in urban areas.
    static std::size_t nUrbanCoefficient_; // $$$$ MCO 2012-08-28: should not be handled by the module
};

}
}

#endif // fire_module_DirectFireData_h
