// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_WeaponDataType_DirectFire_h
#define fire_module_WeaponDataType_DirectFire_h

#include "MT_Tools/MT_InterpolatedFunction.h"
#include <boost/noncopyable.hpp>
#include <vector>

namespace xml
{
    class xistream;
}

namespace sword
{
namespace wrapper
{
    class View;
}

namespace fire
{
    class LauncherType;
    class DotationCategory;

// =============================================================================
/** @class  WeaponDataType_DirectFire
    @brief  Weapon data type for direct fire
*/
// Created: JVT 2004-08-03
// =============================================================================
class WeaponDataType_DirectFire : private boost::noncopyable
{
public:
    //! @name Constructors>Destructor
    //@{
    WeaponDataType_DirectFire( const LauncherType& launcherType, const DotationCategory& dotation, xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    double GetDangerosity                ( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH ) const;
    double GetDangerosity                ( const wrapper::View& compTarget, double distance ) const;
    double GetMaxRangeToFireOn           ( const wrapper::View& compTarget, double rWantedPH ) const;
    double GetMinRangeToFireOn           ( const wrapper::View& compTarget, double rWantedPH ) const;
    double GetMaxRangeToFire             ( double rWantedPH ) const;
    double GetMaxRangeToFireOnWithPosture( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, double rWantedPH ) const;
    double GetMinRangeToFireOnWithPosture( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, double rWantedPH ) const;

    void Fire( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH ) const;
    void Fire( const wrapper::View& firer, const wrapper::View& element, unsigned int nNbrAmmoReserved ) const;
    //@}

private:
    //! @name Helpers
    //@{
    double GetPH( const wrapper::View& firer, const wrapper::View& target, std::size_t targetVolume ) const;
    double GetPH( std::size_t targetVolume, double rDistance ) const;

    double GetMaxDistanceForPH( double ph, const wrapper::View& compTarget ) const;
    double GetMinDistanceForPH( double ph, const wrapper::View& compTarget ) const;
    double GetMaxDistanceForPH( const wrapper::View& firer, const wrapper::View& target, double ph, const wrapper::View& compTarget ) const;
    double GetMinDistanceForPH( const wrapper::View& firer, const wrapper::View& target, double ph, const wrapper::View& compTarget ) const;

    void InitializePH( xml::xistream& xis );

    void ReadHitProbability( xml::xistream& xis, MT_InterpolatedFunction< double >& phFunction );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_InterpolatedFunction< double > >  T_PhVector;
    typedef T_PhVector::const_iterator                      CIT_PhVector;
    //@}

private:
    //! @name Member data
    //@{
    const LauncherType& launcherType_;
    const DotationCategory& dotation_;
    T_PhVector phs_;
    //@}
};

}
}

#endif // fire_module_WeaponDataType_DirectFire_h
