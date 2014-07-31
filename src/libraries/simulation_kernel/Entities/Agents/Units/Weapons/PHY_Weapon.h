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

class PHY_WeaponType;
class PHY_Composante_ABC;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_PopulationElement_ABC;
class MIL_Effect_IndirectFire;
class PHY_FireResults_ABC;
class PHY_DotationCategory;
class PHY_ComposanteType_ABC;
class MIL_Time_ABC;
class MT_Vector2D;

// =============================================================================
// @class  PHY_Weapon
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Weapon : private boost::noncopyable
{
public:
             PHY_Weapon( const MIL_Time_ABC& time, const PHY_WeaponType& type, bool bMajor );
    virtual ~PHY_Weapon();

    //! @name Accessors
    //@{
    bool IsReady() const; // $$$ Retourne "pas en train de tirer" ni "pas en train de recharger" - NE TIENT PAS COMPTE DES DOTATIONS - COMMUN DIRECT ET INDIRECT
    bool CanDirectFire() const;
    bool CanIndirectFire() const;
    const PHY_DotationCategory& GetDotationCategory() const;

    const PHY_WeaponType& GetType() const
    {
        return type_;
    }
    bool IsMajor() const
    {
        return bMajor_;
    }
    //@}

    //! @name Operations
    //@{
    bool IndirectFire( MIL_Agent_ABC& firer, MIL_Effect_IndirectFire& effect );
    bool DirectFire( MIL_AgentPion& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH );
    bool DirectFire( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target, PHY_FireResults_ABC& fireResult );
    double ThrowSmoke( MIL_Agent_ABC& firer, const MT_Vector2D& vTargetPosition, PHY_FireResults_ABC& fireResult );
    double GetDangerosity( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH, bool bUseAmmo ) const;
    double GetDangerosity( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& compTarget, double rDistBtwFirerAndTarget, bool bUseAmmo ) const;
    double GetMaxRangeToIndirectFire() const;
    double GetMinRangeToIndirectFire() const;
    int GetNumberOfDotationPerBurst( const PHY_DotationCategory& ) const;
    //@}

private:
    //! @name Tools
    //@{
    double ModifyReloadingDuration( MIL_Agent_ABC& firer, double rDuration ) const;
    //@}

private:
    const MIL_Time_ABC& time_;
    const PHY_WeaponType& type_;
    const bool bMajor_;

    unsigned int nNbrAmmoFiredFromLoader_;
    double rNextTimeStepToFire_;
};

#endif // __PHY_Weapon_h_
