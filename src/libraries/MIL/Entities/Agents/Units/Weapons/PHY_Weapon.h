// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/units/weapons/PHY_Weapon.h $
// $Author: Jvt $
// $Modtime: 2/05/05 17:31 $
// $Revision: 7 $
// $Workfile: PHY_Weapon.h $
//
// *****************************************************************************

#ifndef __PHY_Weapon_h_
#define __PHY_Weapon_h_

#include "MIL.h"

class PHY_WeaponType;
class PHY_WeaponData_DirectFire;
class PHY_WeaponData_IndirectFire;
class PHY_Composante_ABC;
class MIL_AgentPion;
class MIL_Agent_ABC;
class PHY_FireResults_ABC;
class PHY_DotationCategory;
class PHY_ComposanteType_ABC;
class MIL_Effect_IndirectFire;
class PHY_RolePion_Dotations;
class PHY_IndirectFireDotationClass;

// =============================================================================
// @class  PHY_Weapon
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Weapon
{
    MT_COPYNOTALLOWED( PHY_Weapon )

public:
     PHY_Weapon( const PHY_WeaponType& type, bool bMajor );
    ~PHY_Weapon();

    //! @name Accessors
    //@{
          bool                  IsReady            () const; // $$$ Retourne "pas en train de tirer" ni "pas en train de recharger" - NE TIENT PAS COMPTE DES DOTATIONS - COMMUN DIRECT ET INDIRECT
          bool                  CanDirectFire      () const;
          bool                  CanIndirectFire    () const;
    const PHY_DotationCategory& GetDotationCategory() const;
    //@}

    //! @name Operations
    //@{
    bool     IndirectFire             ( MIL_AgentPion& firer, MIL_Effect_IndirectFire& effect );
    bool     DirectFire               ( MIL_AgentPion& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH );
    void     ThrowSmoke               ( MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmo ) const;
    MT_Float GetDangerosity           ( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH ) const;
    MT_Float GetDangerosity           ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& compTarget, MT_Float rDistBtwFirerAndTarget ) const;
    MT_Float GetMaxRangeToFireOn      ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;
    MT_Float GetMinRangeToFireOn      ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;
    MT_Float GetMaxRangeToIndirectFire() const;
    MT_Float GetMinRangeToIndirectFire() const;
    //@}

private:
    //! @name Tools
    //@{
    MT_Float ModifyReloadingDuration( const MIL_AgentPion& firer, MT_Float rDuration ) const;
    //@}

private:
    const PHY_WeaponType& type_;
    const bool            bMajor_;

    uint        nNbrAmmoFiredFromLoader_;
    MT_Float    rNextTimeStepToFire_;
};

#include "PHY_Weapon.inl"

#endif // __PHY_Weapon_h_
