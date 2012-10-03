// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_WeaponType_h
#define fire_module_WeaponType_h

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <map>

class MT_Vector2D;

namespace xml
{
    class xistream;
    class xisubstream;
}

namespace sword
{
namespace wrapper
{
    class View;
}

namespace fire
{
    class WeaponDataType_DirectFire;
    class WeaponDataType_IndirectFire;
    class LauncherType;
    class DotationCategory;

// =============================================================================
/** @class  WeaponType
    @brief  Weapon type
*/
// Created: JVT 2004-08-03
// =============================================================================
class WeaponType : private boost::noncopyable
{
public:
    //! @name Operations
    //@{
    static void Initialize( xml::xisubstream xis, double tickDuration );

    static boost::shared_ptr< WeaponType > FindWeaponType( const std::string& strType );
    //@}

    //! @name Accessors
    //@{
    unsigned int GetNbrAmmoPerLoader() const;
    unsigned int GetNbrAmmoPerBurst() const;
    double GetBurstDuration() const;
    double GetReloadingDuration() const;
    bool CanDirectFire( const wrapper::View& component, const wrapper::View& parameters ) const;
    bool CanIndirectFire( const wrapper::View& entity, const wrapper::View& component, const std::string& type, const MT_Vector2D* target ) const;
    bool HasDotation( const wrapper::View& firer ) const;
    const DotationCategory& GetDotation() const;
    //@}

    //! @name Operations
    //@{
    unsigned int ReserveAmmunition( const wrapper::View& firer, unsigned int nNbrAmmoToFire );

//    void     IndirectFire       ( MIL_Effect_IndirectFire& effect, unsigned int nNbrAmmoReserved ) const;
    void     DirectFire         ( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH ) const;
    void     DirectFire         ( const wrapper::View& firer, const wrapper::View& element, unsigned int nNbrAmmoReserved ) const;
//    void     ThrowSmoke         ( MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo, PHY_FireResults_ABC& fireResult ) const;

    double GetDangerosity     ( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH, bool checkAmmo ) const;
    double GetDangerosity     ( const wrapper::View& firer, const wrapper::View& target, double distance, bool checkAmmo ) const;

    double GetMaxRangeToFireOn( const wrapper::View& firer, const wrapper::View& target, double rWantedPH, const char* dotation ) const;
    double GetMinRangeToFireOn( const wrapper::View& firer, const wrapper::View& target, double rWantedPH ) const;
    double GetMaxRangeToFire( double rWantedPH ) const;

    double GetMaxRangeToFireOnWithPosture( const wrapper::View& firer, const wrapper::View& target, double rWantedPH ) const;
    double GetMinRangeToFireOnWithPosture( const wrapper::View& firer, const wrapper::View& target, double rWantedPH ) const;

    double GetMaxRangeToIndirectFire( const wrapper::View& firer, boost::optional< std::string > dotation, bool checkAmmo ) const;
    double GetMinRangeToIndirectFire( const wrapper::View& firer, boost::optional< std::string > dotation, bool checkAmmo ) const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     WeaponType( const std::string& strLauncher, const std::string& strAmmunition, xml::xistream& xis, double tickDuration );
    //@}

     //! @name Helpers
    //@{
    static void ReadWeapon( xml::xistream& xis, double tickDuration );
    void ReadDirect       ( xml::xistream& xis );
    void ReadIndirect     ( xml::xistream& xis, double tickDuration );
    bool CheckDirectFireDotation( const wrapper::View& firer, bool checkAmmo ) const;
    bool CheckIndirectFireDotation( const wrapper::View& firer, boost::optional< std::string > dotation, bool checkAmmo ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< WeaponType > > T_WeaponTypeMap;
    typedef T_WeaponTypeMap::const_iterator                        CIT_WeaponTypeMap;
    //@}

private:
    //! @name Member data
    //@{
    const boost::shared_ptr< LauncherType > pLauncherType_;
    const boost::shared_ptr< DotationCategory > dotation_;
    unsigned int nNbrAmmoPerBurst_;
    double rBurstDuration_;
    unsigned int nNbrAmmoPerLoader_;
    double rReloadingDuration_;
    std::auto_ptr< WeaponDataType_DirectFire > pDirectFireData_;
    std::auto_ptr< WeaponDataType_IndirectFire > pIndirectFireData_;
    static T_WeaponTypeMap weaponTypes_;
    //@}
};

}
}

#endif // fire_module_WeaponType_h
