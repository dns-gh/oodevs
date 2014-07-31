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
    static void Initialize( const MIL_Time_ABC& time, xml::xistream& xis );
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
          double              GetBurstDuration    () const;
          double              GetReloadingDuration() const;
    const std::string&          GetName             () const;
          bool                  CanDirectFire       () const;
          bool                  CanIndirectFire     () const;
    std::size_t GetID() const;
    //@}

    //! @name Operations
    //@{
    void     IndirectFire       ( MIL_Effect_IndirectFire& effect, unsigned int nNbrAmmoReserved ) const;
    void     DirectFire         ( MIL_AgentPion& firer, MIL_PopulationElement_ABC& target, unsigned int nNbrAmmoReserved, PHY_FireResults_ABC& fireResult ) const;
    void     DirectFire         ( MIL_AgentPion& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH ) const;
    void     ThrowSmoke         ( MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo, PHY_FireResults_ABC& fireResult ) const;
    double   GetSmokeDuration() const;

    double GetDangerosity     ( const MIL_AgentPion& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& compTarget, bool bUsePH, bool bUseAmmo ) const;
    double GetDangerosity     ( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rDistBtwFirerAndTarget, bool bUseAmmo ) const;

    double GetMaxRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH, const PHY_DotationCategory* dotation, bool useAmmo ) const;
    double GetMinRangeToFireOn( const MIL_Agent_ABC& firer, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const;

    double GetMaxRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const;
    double GetMinRangeToFireOnWithPosture( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const;
    double GetMaxRangeToFire( const MIL_Agent_ABC& firer, double rWantedPH ) const;

    double GetMaxRangeToDirectFire() const;

    double GetMaxRangeToIndirectFire( const MIL_Agent_ABC& firer, bool bCheckDotationsAvailability ) const;
    double GetMinRangeToIndirectFire( const MIL_Agent_ABC& firer, bool bCheckDotationsAvailability ) const;
    double GetMaxRangeToIndirectFire() const;
    double GetMinRangeToIndirectFire() const;

    double GetPHModificator( const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture ) const;
    double GetPHModificator() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, std::string >              T_WeaponTypeKey;
    typedef std::map< T_WeaponTypeKey, const PHY_WeaponType* > T_WeaponTypeMap;
    //@}

private:
     PHY_WeaponType( const MIL_Time_ABC& time, const std::string& strLauncher, const std::string& strAmmunition, size_t identifier, xml::xistream& xis );
    ~PHY_WeaponType();

    //! @name Initialization
    //@{
    void InitializeDirectFireData  ( xml::xistream& xis );
    void InitializeIndirectFireData( xml::xistream& xis, double timeFactor );
    //@}

    //! @name Helpers
    //@{
    static void ReadWeapon( xml::xistream& xis, const MIL_Time_ABC& time );
    void ReadDirect       ( xml::xistream& xis );
    void ReadIndirect     ( xml::xistream& xis, double timeFactor );
    //@}

private:
    const MIL_Time_ABC&         time_;
    const PHY_LauncherType*     pLauncherType_;
    const PHY_DotationCategory* pDotationCategory_;
          std::string           strName_;
          std::size_t           identifier_;

          unsigned int                  nNbrAmmoPerBurst_;
          double              rBurstDuration_;
          unsigned int                  nNbrAmmoPerLoader_;
          double              rReloadingDuration_;

    PHY_WeaponDataType_DirectFire*   pDirectFireData_;
    PHY_WeaponDataType_IndirectFire* pIndirectFireData_;

private:
    static T_WeaponTypeMap   weaponTypes_;
};

#endif // __PHY_WeaponType_h_
