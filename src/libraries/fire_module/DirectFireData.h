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

#include "Weapon.h"
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
public:
    //! @name Types
    //@{
    enum E_ComposanteFiringType
    {
        eFireUsingAllComposantes,
        eFireUsingOnlyComposantesLoadable,
        eFireUsingOnlyComposantesCarrier
    };

    enum E_ComposanteFiredType
    {
        eFireOnAllComposantes,
        eFireOnlyOnMajorComposantes
    };

    enum E_FiringMode
    {
        eFiringModeNormal, // Chaque composante fait feu avec le meilleur couple ( lanceur / munition )
        eFiringModeFree    // Chaque composante fait feu avec tous les lanceurs
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    DirectFireData( ModuleFacade& module, const wrapper::View& firer, E_ComposanteFiringType nComposanteFiringType,
        E_FiringMode nFiringMode = eFiringModeNormal, double rPercentageComposantesToUse = 1.,
        int ammoDotationClass = -1 );
    //@}

    //! @name Operations
    //@{
    void ApplyOnWeapon( const wrapper::View& model, const wrapper::View& component, const wrapper::View& weapon );

    void ChooseRandomWeapon  ( const wrapper::View& target, const wrapper::View& compTarget, const SWORD_Model*& pBestFirer, const Weapon*& pBestWeapon ) const;
    void ChooseBestWeapon    ( const wrapper::View& target, const wrapper::View& compTarget, const SWORD_Model*& pBestFirer, const Weapon*& pBestWeapon ) const;
    bool GetUnusedFirerWeapon( const SWORD_Model*& pUnusedFirer, const Weapon*& pUnusedFirerWeapon ) const;
    void ReleaseWeapon       ( const SWORD_Model* firer, const Weapon& weapon );

    bool CanFire( const wrapper::View& firer );
    //@}

    //! @name Accessors
    //@{
    bool HasWeaponsNotReady () const;
    bool HasWeaponsAndNoAmmo() const;
    bool IsTemporarilyBlocked() const;
    unsigned int GetNbrWeaponsUsable() const;
    //@}

private:
    //! @name Tools
    //@{
    void RemoveFirer ( const SWORD_Model* component );
    void RemoveWeapon( const SWORD_Model* component, const Weapon& weapon );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Weapon >            T_WeaponVector;
    typedef T_WeaponVector::const_iterator CIT_WeaponVector;

    struct sComposanteWeapons
    {
         sComposanteWeapons();

        //! @name Accessors
        //@{
        unsigned int GetNbrWeaponsUsable() const;
        bool IsFiring           () const;
        //@}

        //! @name Operations
        //@{
        void        AddWeapon      ( const Weapon& weapon );
        void        RemoveWeapon   ( const Weapon& weapon );
        bool        GetBestWeapon  ( double& rBestScore, const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, const Weapon*& pBestWeapon ) const;
        bool        GetRandomWeapon( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, const Weapon*& pRandomWeapon ) const;
        const Weapon* GetUnusedWeapon() const;
        //@}

    private:
        bool            bIsFiring_;
        T_WeaponVector  weapons_;
    };

    typedef std::map< const SWORD_Model*, sComposanteWeapons > T_ComposanteWeaponsMap;
    typedef T_ComposanteWeaponsMap::const_iterator                    CIT_ComposanteWeaponsMap;
    //@}

private:
    //! @name Member data
    //@{
    ModuleFacade& module_;
    const wrapper::View firer_;
    const int ammoDotationClass_;
    const E_ComposanteFiringType nComposanteFiringType_;
    const E_FiringMode           nFiringMode_;
    const double                 rPercentageComposantesToUse_;
          T_ComposanteWeaponsMap composantesWeapons_;
          bool                   bHasWeaponsReady_;
          bool                   bHasWeaponsNotReady_;
          bool                   bHasWeaponsAndNoAmmo_;
          bool                   bTemporarilyBlocked_;
    //@}

public:
    // Factor applied to number of components that can fire in urban areas.
    static std::size_t nUrbanCoefficient_; // $$$$ MCO 2012-08-28: should not be handled by the module
};

}
}

#endif // fire_module_DirectFireData_h
