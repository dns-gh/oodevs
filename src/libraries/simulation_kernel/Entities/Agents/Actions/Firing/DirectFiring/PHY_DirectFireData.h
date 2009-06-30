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
     PHY_DirectFireData( MIL_AgentPion& firer, E_ComposanteFiringType nComposanteFiringType, E_FiringMode nFiringMode = eFiringModeNormal, MT_Float rPercentageComposantesToUse = 1., const PHY_AmmoDotationClass* pAmmoDotationClass = 0 );
    ~PHY_DirectFireData();

    //! @name Operations
    //@{
    void operator() ( const PHY_ComposantePion& firer, PHY_Weapon& weapon );

    void ChooseRandomWeapon  ( const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, const PHY_ComposantePion*& pBestFirer, PHY_Weapon*& pBestWeapon ) const;
    void ChooseBestWeapon    ( const MIL_Agent_ABC& target, const PHY_Composante_ABC& compTarget, const PHY_ComposantePion*& pBestFirer, PHY_Weapon*& pBestWeapon ) const;
    bool GetUnusedFirerWeapon( const PHY_ComposantePion*& pUnusedFirer, PHY_Weapon*& pUnusedFirerWeapon ) const;
    void ReleaseWeapon       ( const PHY_ComposantePion& firer, PHY_Weapon& weapon );
    //@}

    //! @name Accessors
    //@{
    bool HasWeaponsNotReady () const;
    bool HasWeaponsAndNoAmmo() const;
    uint GetNbrWeaponsUsable() const;
    //@}

private:
    //! @name Tools
    //@{
    void RemoveFirer ( const PHY_ComposantePion& firer );
    void RemoveWeapon( const PHY_ComposantePion& firer, PHY_Weapon& weapon );
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

    typedef std::map< const PHY_ComposantePion*, sComposanteWeapons > T_ComposanteWeaponsMap;
    typedef T_ComposanteWeaponsMap::const_iterator                    CIT_ComposanteWeaponsMap;
    //@}

private:
          MIL_AgentPion&         firer_;
    const PHY_AmmoDotationClass* pAmmoDotationClass_;
    const E_ComposanteFiringType nComposanteFiringType_;
    const E_FiringMode           nFiringMode_;
    const MT_Float               rPercentageComposantesToUse_;
          T_ComposanteWeaponsMap composantesWeapons_;
          bool                   bHasWeaponsReady_;
          bool                   bHasWeaponsNotReady_;
          bool                   bHasWeaponsAndNoAmmo_;

private:
    static MT_Random randomGenerator_;
};

#endif // __PHY_DirectFireData_h_
