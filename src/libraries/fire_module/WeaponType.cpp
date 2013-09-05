// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "WeaponType.h"
#include "LauncherType.h"
#include "DotationCategory.h"
#include "WeaponDataType_DirectFire.h"
#include "WeaponDataType_IndirectFire.h"
#include "MT_Tools/MT_Vector2D.h"
#include "tools/Codec.h"
#include <tools/Exception.h>
#include <wrapper/Hook.h>
#include <wrapper/View.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.hpp>
#include <boost/optional.hpp>
#include <boost/bind.hpp>
#include <cassert>

DEFINE_HOOK( InitializeWeaponSystems, 2, void, ( const char* xml, double tickDuration ) )
{
    // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
    sword::fire::WeaponType::Initialize( xml::xistringstream( xml ), tickDuration );
    if( GET_PREVIOUS_HOOK( InitializeWeaponSystems ) )
        GET_PREVIOUS_HOOK( InitializeWeaponSystems )( xml, tickDuration );
}

using namespace sword;
using namespace sword::fire;

// -----------------------------------------------------------------------------
// Name: WeaponType constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
WeaponType::WeaponType( const std::string& launcher, const std::string& ammunition, xml::xistream& xis, double tickDuration )
    : pLauncherType_     ( LauncherType::FindLauncherType( launcher ) )
    , dotation_          ( DotationCategory::FindDotationCategory( ammunition ) )
    , nNbrAmmoPerBurst_  ( 1 )
    , rBurstDuration_    ( 1. )
    , nNbrAmmoPerLoader_ ( 1 )
    , rReloadingDuration_( 1. )
{
    if( ! pLauncherType_ )
        throw xml::exception( xis.context() + "Unknown launcher type '" + launcher + "'" );
    if( ! dotation_ )
        throw xml::exception( xis.context() + "Unknown dotation category '" + ammunition + "'" );
    std::string burstTime, reloadingTime;
    xis >> xml::start( "burst" )
            >> xml::attribute( "munition", nNbrAmmoPerBurst_ )
            >> xml::attribute( "duration", burstTime )
        >> xml::end
        >> xml::start( "reloading" )
            >> xml::attribute( "munition", nNbrAmmoPerLoader_ )
            >> xml::attribute( "duration", reloadingTime )
        >> xml::end;
    if( ! tools::DecodeTime( burstTime,     rBurstDuration_ )
     || ! tools::DecodeTime( reloadingTime, rReloadingDuration_ ) )
        throw xml::exception( xis.context() + "Invalid burst or reloading durations" );
    rBurstDuration_     /= tickDuration;
    rReloadingDuration_ /= tickDuration;
    if( nNbrAmmoPerBurst_ <= 0 )
        throw xml::exception( xis.context() + "burst: munition <= 0" );
    if( rBurstDuration_ <= 0 )
        throw xml::exception( xis.context() + "burst: duration <= 0" );
    if( nNbrAmmoPerLoader_ <= 0 )
        throw xml::exception( xis.context() + "reloading: munition <= 0" );
    if( rReloadingDuration_ <= 0 )
        throw xml::exception( xis.context() + "reloading: duration <= 0" );
    xis >> xml::list( "direct-fire", *this, &WeaponType::ReadDirect )
        >> xml::list( "indirect-fire", *this, &WeaponType::ReadIndirect, tickDuration );
}

namespace
{
    std::vector< boost::shared_ptr< WeaponType > > types;
}

// -----------------------------------------------------------------------------
// Name: WeaponType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void WeaponType::Initialize( xml::xisubstream xis, double tickDuration )
{
    ::SWORD_Log( SWORD_LOG_LEVEL_INFO, "Initializing weapons" );
    xis >> xml::start( "weapons" )
            >> xml::list( "weapon-system", boost::bind( &WeaponType::ReadWeapon, _1, tickDuration ) );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::ReadWeapon
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void WeaponType::ReadWeapon( xml::xistream& xis, double tickDuration )
{
    std::string launcher, ammunition;
    xis >> xml::attribute( "launcher", launcher )
        >> xml::attribute( "munition", ammunition );
    types.push_back( boost::shared_ptr< WeaponType >( new WeaponType( launcher, ammunition, boost::ref( xis ), tickDuration ) ) );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::FindWeaponType
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
boost::shared_ptr< WeaponType > WeaponType::FindWeaponType( std::size_t type )
{
    if( type >= types.size() )
        throw MASA_EXCEPTION( "Unknown weapon type : " + boost::lexical_cast< std::string >( type ) );
    return types[ type ];
}

// -----------------------------------------------------------------------------
// Name: WeaponType::ReadDirect
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void WeaponType::ReadDirect( xml::xistream& xis )
{
    assert( pLauncherType_ );
    assert( dotation_ );
    if( ! pLauncherType_->CanDirectFire() )
        throw xml::exception( xis.context() + "Associated launcher cannot direct fire" );
    if( ! dotation_->CanBeUsedForDirectFire() )
        throw xml::exception( xis.context() + "Associated ammunition cannot direct fire" );
    pDirectFireData_.reset( new WeaponDataType_DirectFire( *pLauncherType_, *dotation_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::ReadIndirect
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void WeaponType::ReadIndirect( xml::xistream& xis, double tickDuration )
{
    assert( pLauncherType_ );
    assert( dotation_ );
    if( ! pLauncherType_->CanIndirectFire() )
        throw xml::exception( xis.context() + "Associated launcher cannot indirect fire" );
    if( ! dotation_->CanBeUsedForIndirectFire() )
        throw xml::exception( xis.context() + "Associated ammunition cannot indirect fire" );
    pIndirectFireData_.reset( new WeaponDataType_IndirectFire( xis, tickDuration ) );
}

//// -----------------------------------------------------------------------------
//// Name: WeaponType::IndirectFire
//// Created: NLD 2004-10-15
//// -----------------------------------------------------------------------------
//void WeaponType::IndirectFire( MIL_Effect_IndirectFire& effect, unsigned int nNbrAmmoReserved ) const
//{
//    assert( pIndirectFireData_ );
//    pIndirectFireData_->Fire( effect, nNbrAmmoReserved );
//}

// -----------------------------------------------------------------------------
// Name: WeaponType::DirectFire
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void WeaponType::DirectFire( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH ) const
{
    assert( pDirectFireData_.get() );
    pDirectFireData_->Fire( firer, target, compTarget, bUsePH );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::DirectFire
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void WeaponType::DirectFire( const wrapper::View& firer, const wrapper::View& element, std::size_t ammos ) const
{
    assert( pDirectFireData_.get() );
    pDirectFireData_->Fire( firer, element, ammos );
}

//// -----------------------------------------------------------------------------
//// Name: WeaponType::ThrowSmoke
//// Created: NLD 2004-10-21
//// -----------------------------------------------------------------------------
//void WeaponType::ThrowSmoke( MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo, PHY_FireResults_ABC& fireResult ) const
//{
//    assert( pIndirectFireData_ );
//    pIndirectFireData_->ThrowSmoke( firer, vSourcePosition, vTargetPosition, nNbrAmmo, fireResult );
//}

// -----------------------------------------------------------------------------
// Name: WeaponType::CheckDirectFireDotation
// Created: MCO 2012-05-09
// -----------------------------------------------------------------------------
bool WeaponType::CheckDirectFireDotation( const wrapper::View& firer, bool checkAmmo ) const
{
    if( ! pDirectFireData_.get() )
        return false;
    if( ! checkAmmo )
        return true;
    return dotation_->HasDotation( firer );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::CheckIndirectFireDotation
// Created: MCO 2012-06-22
// -----------------------------------------------------------------------------
bool WeaponType::CheckIndirectFireDotation( const wrapper::View& firer, int dotation, bool checkAmmo ) const
{
    if( ! pIndirectFireData_.get() )
        return false;
    if( ! dotation )
        return true;
    if( DotationCategory::FindDotationCategory( dotation ) != dotation_ )
        return false;
    if( ! checkAmmo )
        return true;
    return dotation_->HasDotation( firer );
}

namespace
{
    template< typename Accumulator, typename Filter, typename Operation >
    double Get( Accumulator accumulator, const wrapper::View& firer, Filter filter, Operation operation, double init )
    {
        double result = init;
        const wrapper::View& components = firer[ "components" ];
        for( std::size_t c = 0; c < components.GetSize(); ++c )
        {
            const wrapper::View& component = components.GetElement( c );
            if( filter( component ) )
            {
                const wrapper::View& weapons = component[ "weapons" ];
                for( std::size_t w = 0; w < weapons.GetSize(); ++w )
                {
                    boost::shared_ptr< WeaponType > type = WeaponType::FindWeaponType( weapons.GetElement( w )[ "type" ] );
                    result = accumulator( result, operation( type ) );
                }
            }
        }
        return result;
    }
    template< typename Filter, typename Operation >
    double GetMax( const wrapper::View& firer, Filter filter, Operation operation, double init = 0 )
    {
        return Get( &std::max< double >, firer, filter, operation, init );
    }
    template< typename Filter, typename Operation >
    double GetMin( const wrapper::View& firer, Filter filter, Operation operation, double init = std::numeric_limits< double >::max() )
    {
        return Get( &std::min< double >, firer, filter, operation, init );
    }
}

DEFINE_HOOK( GetDangerosity, 5, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double distance, bool checkAmmo ) )
{
    return GetMax( firer, filter, boost::bind( &WeaponType::GetDangerosity, _1, firer, target, distance, checkAmmo ) );
}
DEFINE_HOOK( GetMaxRangeToFireOn, 5, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH, int dotation ) )
{
    return GetMax( firer, filter, boost::bind( &WeaponType::GetMaxRangeToFireOn, _1, firer, target, rWantedPH, dotation ) );
}
DEFINE_HOOK( GetMinRangeToFireOn, 4, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
{
    return GetMin( firer, filter, boost::bind( &WeaponType::GetMinRangeToFireOn, _1, firer, target, rWantedPH ) );
}
DEFINE_HOOK( GetMaxRangeToFire, 3, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
{
    return GetMax( firer, filter, boost::bind( &WeaponType::GetMaxRangeToFire, _1, rWantedPH ) );
}
DEFINE_HOOK( GetMaxRangeToFireOnWithPosture, 4, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
{
    return GetMax( firer, filter, boost::bind( &WeaponType::GetMaxRangeToFireOnWithPosture, _1, firer, target, rWantedPH ) );
}
DEFINE_HOOK( GetMinRangeToFireOnWithPosture, 4, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
{
    return GetMin( firer, filter, boost::bind( &WeaponType::GetMinRangeToFireOnWithPosture, _1, firer, target, rWantedPH ) );
}
DEFINE_HOOK( GetMaxRangeToIndirectFire, 4, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), int dotation, bool checkAmmo ) )
{
    return GetMax( firer, filter, boost::bind( &WeaponType::GetMaxRangeToIndirectFire, _1, firer, dotation, checkAmmo ), -1 );
}
DEFINE_HOOK( GetMinRangeToIndirectFire, 4, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), int dotation, bool checkAmmo ) )
{
    return GetMin( firer, filter, boost::bind( &WeaponType::GetMinRangeToIndirectFire, _1, firer, dotation, checkAmmo ) );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double WeaponType::GetDangerosity( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH, bool checkAmmo ) const
{
    if( CheckDirectFireDotation( firer, checkAmmo ) )
        return pDirectFireData_->GetDangerosity( firer, target, compTarget, bUsePH );
    return 0;
}

namespace
{
    boost::optional< wrapper::View > GetMajorComponent( const wrapper::View& target )
    {
        const int major = target[ "major" ];
        if( major == -1 )
            return boost::none;
        return target[ "components" ].GetElement( major );
    }
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double WeaponType::GetDangerosity( const wrapper::View& firer, const wrapper::View& target, double distance, bool checkAmmo ) const
{
    if( ! pDirectFireData_.get() || pDirectFireData_->GetMaxRange() < distance )
        return 0;
    boost::optional< wrapper::View > component = GetMajorComponent( target );
    if( component && CheckDirectFireDotation( firer, checkAmmo ) )
        return pDirectFireData_->GetDangerosity( *component, distance );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double WeaponType::GetMaxRangeToFireOn( const wrapper::View& firer, const wrapper::View& target, double rWantedPH, int dotation ) const
{
    if( ! pDirectFireData_.get() )
        return 0;
    const int major = target[ "major" ];
    if( major == -1 )
        return 0;
    boost::shared_ptr< DotationCategory > category = DotationCategory::FindDotationCategory( dotation );
    if( category ) // $$$$ MCO 2012-05-09: weird that we don't check if firer has enough ammo in the else case
    {
        if( category != dotation_ )
            return 0;
        if( ! dotation_->HasDotation( firer ) )
            return 0;
    }
    return pDirectFireData_->GetMaxRangeToFireOn( target[ "components" ].GetElement( major ), rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
double WeaponType::GetMinRangeToFireOn( const wrapper::View& firer, const wrapper::View& target, double rWantedPH ) const
{
    boost::optional< wrapper::View > component = GetMajorComponent( target );
    if( component && CheckDirectFireDotation( firer, true ) )
        return pDirectFireData_->GetMinRangeToFireOn( *component, rWantedPH );
    return std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetMaxRangeToFire
// Created: DDA 2010-05-03
// -----------------------------------------------------------------------------
double WeaponType::GetMaxRangeToFire( double rWantedPH ) const
{
    if( pDirectFireData_.get() )
        return pDirectFireData_->GetMaxRangeToFire( rWantedPH );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double WeaponType::GetMaxRangeToFireOnWithPosture( const wrapper::View& firer, const wrapper::View& target, double rWantedPH ) const
{
    boost::optional< wrapper::View > component = GetMajorComponent( target );
    if( component && CheckDirectFireDotation( firer, true ) )
        return pDirectFireData_->GetMaxRangeToFireOnWithPosture( firer, target, *component, rWantedPH );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double WeaponType::GetMinRangeToFireOnWithPosture( const wrapper::View& firer, const wrapper::View& target, double rWantedPH ) const
{
    boost::optional< wrapper::View > component = GetMajorComponent( target );
    if( component && CheckDirectFireDotation( firer, true ) )
        return pDirectFireData_->GetMinRangeToFireOnWithPosture( firer, target, *component, rWantedPH );
    return std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetMaxRangeToIndirectFire
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
double WeaponType::GetMaxRangeToIndirectFire( const wrapper::View& firer, int dotation, bool checkAmmo ) const
{
    if( CheckIndirectFireDotation( firer, dotation, checkAmmo ) )
        return pIndirectFireData_->GetMaxRange();
    return -1;
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetMinRangeToIndirectFire
// Created: MCO 2012-06-25
// -----------------------------------------------------------------------------
double WeaponType::GetMinRangeToIndirectFire( const wrapper::View& firer, int dotation, bool checkAmmo ) const
{
    if( CheckIndirectFireDotation( firer, dotation, checkAmmo ) )
        return pIndirectFireData_->GetMinRange();
    return std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetNbrAmmoPerLoader
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
unsigned int WeaponType::GetNbrAmmoPerLoader() const
{
    return nNbrAmmoPerLoader_;
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetNbrAmmoPerBurst
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
unsigned int WeaponType::GetNbrAmmoPerBurst() const
{
    return nNbrAmmoPerBurst_;
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetBurstDuration
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
double WeaponType::GetBurstDuration() const
{
    return rBurstDuration_;
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetReloadingDuration
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
double WeaponType::GetReloadingDuration() const
{
    return rReloadingDuration_;
}

// -----------------------------------------------------------------------------
// Name: WeaponType::CanDirectFire
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
bool WeaponType::CanDirectFire( const wrapper::View& component, const wrapper::View& parameters ) const
{
    return pDirectFireData_.get() && dotation_->CanFire( component, parameters );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::CanIndirectFire
// Created: MCO 2012-06-27
// -----------------------------------------------------------------------------
bool WeaponType::CanIndirectFire( const wrapper::View& entity, const wrapper::View& component, const std::string& type, const MT_Vector2D* target ) const
{
    if( ! pIndirectFireData_.get() || ! dotation_->CanFire( component, type ) )
        return false;
    if( ! target )
        return true;
    const wrapper::View& movement = entity[ "movement" ];
    const MT_Vector2D position( movement[ "position/x" ], movement[ "position/y" ] );
    const double range = position.Distance( *target );
    return pIndirectFireData_->GetMinRange() <= range && range <= pIndirectFireData_->GetMaxRange();
}

// -----------------------------------------------------------------------------
// Name: WeaponType::ReserveAmmunition
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
std::size_t WeaponType::ReserveAmmunition( const wrapper::View& firer, std::size_t ammos )
{
    return dotation_->ReserveAmmunition( firer, ammos );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::HasDotation
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
bool WeaponType::HasDotation( const wrapper::View& firer ) const
{
    return dotation_->HasDotation( firer );
}

// -----------------------------------------------------------------------------
// Name: WeaponType::GetDotation
// Created: MCO 2012-06-28
// -----------------------------------------------------------------------------
const DotationCategory& WeaponType::GetDotation() const
{
    return *dotation_;
}
