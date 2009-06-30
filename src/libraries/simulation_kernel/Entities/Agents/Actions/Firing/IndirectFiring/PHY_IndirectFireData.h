// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_IndirectFireData.h $
// $Author: Jvt $
// $Modtime: 2/05/05 17:13 $
// $Revision: 3 $
// $Workfile: PHY_IndirectFireData.h $
//
// *****************************************************************************

#ifndef __PHY_IndirectFireData_h_
#define __PHY_IndirectFireData_h_

class MIL_AgentPion;
class MIL_Effect_IndirectFire;
class PHY_ComposantePion;
class PHY_Weapon;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_IndirectFireData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_IndirectFireData
{
    MT_COPYNOTALLOWED( PHY_IndirectFireData )

public:
     PHY_IndirectFireData( MIL_AgentPion& firer, MIL_Effect_IndirectFire& effect );
    ~PHY_IndirectFireData();

    //! @name Operations
    //@{
    void operator()          ( const PHY_ComposantePion& firer, PHY_Weapon& weapon );
    void RemoveFirer         ( const PHY_ComposantePion& firer );
    bool GetUnusedFirerWeapon( const PHY_ComposantePion*& pUnusedFirer, PHY_Weapon*& pUnusedFirerWeapon ) const;
    //@}

    //! @name Accessors
    //@{
    bool HasWeaponsReady    () const;
    bool HasWeaponsNotReady () const;
    bool HasWeaponsAndNoAmmo() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_Weapon* >     T_WeaponVector;
    typedef T_WeaponVector::iterator       IT_WeaponVector;
    typedef T_WeaponVector::const_iterator CIT_WeaponVector;

    struct sComposanteWeapons
    {
         sComposanteWeapons();
        ~sComposanteWeapons();

        //! @name Accessors
        //@{
        bool HasWeaponsReady() const;
        bool IsFiring       () const;
        //@}

        //! @name Operations
        //@{
        void        AddWeapon      ( PHY_Weapon& weapon );
        PHY_Weapon* GetUnusedWeapon() const;
        //@}

    private:
        bool            bIsFiring_;
        T_WeaponVector  weaponsReady_;
        T_WeaponVector  weapons_;
    };

    typedef std::map< const PHY_ComposantePion*, sComposanteWeapons > T_ComposanteWeaponsMap;
    typedef T_ComposanteWeaponsMap::const_iterator                    CIT_ComposanteWeaponsMap;
    //@}

private:
    MIL_AgentPion&           firer_;
    MIL_Effect_IndirectFire& effect_;
    T_ComposanteWeaponsMap   composantesWeapons_;
    bool                     bHasWeaponsReady_;
    bool                     bHasWeaponsNotReady_;
    bool                     bHasWeaponsAndNoAmmo_;
};

#endif // __PHY_IndirectFireData_h_
