// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_WeaponType.h $
// $Author: Jvt $
// $Modtime: 2/05/05 17:35 $
// $Revision: 8 $
// $Workfile: PHY_WeaponType.h $
//
// *****************************************************************************

#ifndef __PHY_WeaponType_h_
#define __PHY_WeaponType_h_

#include "MT_Tools/MT_Tools_Types.h"
#include "MT_Tools/MT_Stl.h"

namespace xml
{
    class xistream;
}

class PHY_LauncherType;
class PHY_DotationCategory;
class PHY_WeaponDataType_IndirectFire;
class PHY_WeaponDataType_DirectFire;
class PHY_Weapon;
class PHY_Posture;
class PHY_RoleInterface_Posture;
class MIL_Effect_IndirectFire;
class MIL_AgentPion;
class MIL_Agent_ABC;
class MIL_PopulationElement_ABC;
class PHY_ComposanteType_ABC;
class PHY_Composante_ABC;
class PHY_FireResults_ABC;
class MIL_Time_ABC;
class MIL_EffectManager;
class MT_Vector2D;

// =============================================================================
// @class  PHY_WeaponType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_WeaponType : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize( const MIL_Time_ABC& time, xml::xistream& xis, double timeFactor );
    static void Terminate ();

    static const PHY_WeaponType* FindWeaponType( const std::string& strLauncher, const std::string& strAmmunition );
    //@}

    //! @name Instanciation
    //@{
    PHY_Weapon& InstanciateWeapon( bool bMajor ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetDotationCategory () const;
          unsigned int                  GetNbrAmmoPerLoader () const;
          unsigned int                  GetNbrAmmoPerBurst  () const;
          MT_Float              GetBurstDuration    () const;
          MT_Float              GetReloadingDuration() const;
    const std::string&          GetName             () const;
          bool                  CanDirectFire       () const;
          bool                  CanIndirectFire     () const;
    //@}

    //! @name Operations
    //@{
    void     IndirectFire       ( MIL_Effect_IndirectFire& effect, unsigned int nNbrAmmoReserved ) const;
    void     DirectFire         ( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target, unsigned int nNbrAmmoReserved, PHY_FireResults_ABC& fireResult ) const;
    void     DirectFire         ( MIL_AgentPion& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH ) const;
    void     ThrowSmoke         ( MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo, PHY_FireResults_ABC& fireResult ) const;

    MT_Float GetDangerosity     ( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& compTarget, bool bUsePH ) const;
    MT_Float GetDangerosity     ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rDistBtwFirerAndTarget ) const;

    MT_Float GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;
    MT_Float GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;

    MT_Float GetMaxRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;
    MT_Float GetMinRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, MT_Float rWantedPH ) const;
    MT_Float GetMaxRangeToFire( const MIL_Agent_ABC& firer, MT_Float rWantedPH ) const;

    MT_Float GetMaxRangeToIndirectFire( const MIL_Agent_ABC& firer, bool bCheckDotationsAvailability ) const;
    MT_Float GetMinRangeToIndirectFire( const MIL_Agent_ABC& firer, bool bCheckDotationsAvailability ) const;
    MT_Float GetMaxRangeToIndirectFire() const;
    MT_Float GetMinRangeToIndirectFire() const;

    MT_Float GetPHModificator( const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture ) const;
    MT_Float GetPHModificator( const PHY_Posture&          firerPosture, const PHY_Posture&               targetPosture ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, std::string >                                    T_WeaponTypeKey;
    typedef std::map< T_WeaponTypeKey, const PHY_WeaponType*, sCaseInsensitiveLess > T_WeaponTypeMap;
    typedef T_WeaponTypeMap::const_iterator                                          CIT_WeaponTypeMap;
    //@}

private:
     PHY_WeaponType( const MIL_Time_ABC& time, const std::string& strLauncher, const std::string& strAmmunition, xml::xistream& xis, double timeFactor );
    ~PHY_WeaponType();

    //! @name Initialization
    //@{
    void InitializeDirectFireData  ( xml::xistream& xis );
    void InitializeIndirectFireData( xml::xistream& xis, double timeFactor );
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadWeapon( xml::xistream& xis, const MIL_Time_ABC& time, double timeFactor );
    void ReadDirect       ( xml::xistream& xis );
    void ReadIndirect     ( xml::xistream& xis, double timeFactor );
    //@}

private:
    const MIL_Time_ABC&         time_;
    const PHY_LauncherType*     pLauncherType_;
    const PHY_DotationCategory* pDotationCategory_;
          std::string           strName_;

          unsigned int                  nNbrAmmoPerBurst_;
          MT_Float              rBurstDuration_;
          unsigned int                  nNbrAmmoPerLoader_;
          MT_Float              rReloadingDuration_;

    PHY_WeaponDataType_DirectFire*   pDirectFireData_;
    PHY_WeaponDataType_IndirectFire* pIndirectFireData_;

private:
    static T_WeaponTypeMap   weaponTypes_;
};

#endif // __PHY_WeaponType_h_
