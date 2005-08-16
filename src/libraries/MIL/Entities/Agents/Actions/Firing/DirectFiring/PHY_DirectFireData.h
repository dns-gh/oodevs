// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/DirectFiring/PHY_DirectFireData.h $
// $Author: Nld $
// $Modtime: 30/11/04 15:54 $
// $Revision: 3 $
// $Workfile: PHY_DirectFireData.h $
//
// *****************************************************************************

#ifndef __PHY_DirectFireData_h_
#define __PHY_DirectFireData_h_

#include "MIL.h"

#include "MT_Tools/MT_Random.h"

class PHY_Composante_ABC;
class PHY_ComposantePion;
class PHY_Weapon;
class PHY_AmmoDotationClass;
class MIL_AgentPion;
class MIL_Agent_ABC;

// =============================================================================
// @class  PHY_DirectFireData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DirectFireData
{
    MT_COPYNOTALLOWED( PHY_DirectFireData )

public:
    //! @name Types
    //@{

    enum E_ComposanteFiringType
    {
        eFireAllComposantes,
        eFireComposantesLoadable,
        eFireComposantesCarrier        
    };
    //@}

public:
     PHY_DirectFireData( MIL_AgentPion& firer, E_ComposanteFiringType nComposanteFiringType, const PHY_AmmoDotationClass* pAmmoDotationClass = 0 );
    ~PHY_DirectFireData();

    //! @name Operations
    //@{
    void AddWeapon   ( PHY_ComposantePion& firer, PHY_Weapon& weapon );
    void RemoveFirer ( PHY_ComposantePion& firer );
    void RemoveWeapon( PHY_ComposantePion& firer, PHY_Weapon& weapon );

    void ChooseRandomWeapon  ( const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, PHY_ComposantePion*& pBestFirer, PHY_Weapon*& pBestWeapon ) const;
    void ChooseBestWeapon    ( const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, PHY_ComposantePion*& pBestFirer, PHY_Weapon*& pBestWeapon ) const;
    bool GetUnusedFirerWeapon( PHY_ComposantePion*& pUnusedFirer, PHY_Weapon*& pUnusedFirerWeapon ) const;
    //@}

    //! @name Accessors
    //@{
    bool HasWeaponsReady    () const;
    bool HasWeaponsNotReady () const;
    bool HasWeaponsAndNoAmmo() const;

    uint GetNbrWeaponsUsable( MT_Float rPercentage ) const; // Mode tir normal
    uint GetNbrWeaponsUsable() const;                       // Mode tir libre
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_Weapon* >     T_WeaponVector;
    typedef T_WeaponVector::const_iterator CIT_WeaponVector;

    struct sComposanteWeapons
    {
         sComposanteWeapons();
        ~sComposanteWeapons();

        //! @name Accessors
        //@{
        bool HasWeaponsReady    () const;
        uint GetNbrWeaponsUsable() const;
        bool IsFiring           () const;
        //@}

        //! @name Operations
        //@{
        void        AddWeapon      ( PHY_Weapon& weapon );
        void        RemoveWeapon   ( PHY_Weapon& weapon );
        bool        GetBestWeapon  ( MT_Float& rBestScore, const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, PHY_Weapon*& pBestWeapon ) const;
        bool        GetRandomWeapon( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, PHY_Weapon*& pRandomWeapon ) const;
        PHY_Weapon* GetUnusedWeapon() const;
        //@}

    private:
        bool            bIsFiring_;
        T_WeaponVector  weaponsReady_;
    };

    typedef std::map< PHY_ComposantePion*, sComposanteWeapons > T_ComposanteWeaponsMap;
    typedef T_ComposanteWeaponsMap::const_iterator              CIT_ComposanteWeaponsMap;
    //@}

private:
          MIL_AgentPion&         firer_;
    const PHY_AmmoDotationClass* pAmmoDotationClass_;
          E_ComposanteFiringType nComposanteFiringType_;
          T_ComposanteWeaponsMap composantesWeapons_;
          bool                   bHasWeaponsReady_;
          bool                   bHasWeaponsNotReady_;
          bool                   bHasWeaponsAndNoAmmo_;

private:
    static MT_Random randomGenerator_;
};

#include "PHY_DirectFireData.inl"

#endif // __PHY_DirectFireData_h_
