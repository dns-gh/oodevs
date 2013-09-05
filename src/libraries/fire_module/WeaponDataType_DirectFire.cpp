// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "WeaponDataType_DirectFire.h"
#include "DotationCategory.h"
#include "LauncherType.h"
#include <tools/Exception.h>
#include <xeumeuleu/xml.hpp>
#include <wrapper/Hook.h>
#include <wrapper/View.h>
#include <wrapper/Event.h>
#include <module_api/Log.h>
#include <boost/lexical_cast.hpp>
#include <cassert>

using namespace sword;
using namespace sword::fire;

DECLARE_HOOK( GetVolumeId, size_t, ( const char* type ) )
DECLARE_HOOK( GetDistance, double, ( const SWORD_Model* firer, const SWORD_Model* target ) )
DECLARE_HOOK( GetFireRandomNumber, double, ( double min, double max ) )
DECLARE_HOOK( GetPopulationElementPh, double, ( const SWORD_Model* firer, const SWORD_Model* element ) )

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
WeaponDataType_DirectFire::WeaponDataType_DirectFire( const LauncherType& launcherType, const DotationCategory& dotation, xml::xistream& xis )
    : launcherType_( launcherType )
    , dotation_    ( dotation )
    , maxRange_    ( 0 )
{
    xis >> xml::list( "hit-probabilities", *this, &WeaponDataType_DirectFire::InitializePH );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::InitializePH
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void WeaponDataType_DirectFire::InitializePH( xml::xistream& xis )
{
    std::string targetType;
    xis >> xml::attribute( "target", targetType );
    std::size_t id = GET_HOOK( GetVolumeId )( targetType.c_str() );
    if( phs_.size() <= id )
        phs_.resize( id + 1 ); // $$$$ MCO 2012-04-30: how to handle unknown type e.g. if ids are not continuous ?
    MT_InterpolatedFunction& function = phs_[ id ];
    function.SetBeforeValue( 0 );
    function.SetAfterValue( 0 );
    xis >> xml::list( "hit-probability", *this, &WeaponDataType_DirectFire::ReadHitProbability, function );
    maxRange_ = std::max( maxRange_, function.GetMaxYForX( 0 ) );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::ReadHitProbability
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void WeaponDataType_DirectFire::ReadHitProbability( xml::xistream& xis, MT_InterpolatedFunction& function )
{
    double distance;
    double ph;
    xis >> xml::attribute( "distance", distance )
        >> xml::attribute( "percentage", ph );
    if( distance < 0 )
        throw xml::exception( xis.context() + "hit-probability: distance <  0" );
    if( ph < 0 || ph > 1 )
        throw xml::exception( xis.context() + "hit-probability: percentage not in [0.1]" );
    function.AddNewPoint( distance, ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMaxDistanceForPH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMaxDistanceForPH( double ph, const wrapper::View& compTarget ) const
{
    const std::size_t targetVolume = compTarget[ "volume" ];
    if( targetVolume >= phs_.size() )
        throw MASA_EXCEPTION( "Invalid target volume identifier in GetMaxDistanceForPH : "
                                  + boost::lexical_cast< std::string >( targetVolume ) );
    return launcherType_.GetPHModificator() * phs_[ targetVolume ].GetMaxYForX( ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMaxDistanceForPH
// Created: MCO 2012-05-09
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMaxDistanceForPH( const wrapper::View& firer, const wrapper::View& target, double ph, const wrapper::View& compTarget ) const
{
    const std::size_t targetVolume = compTarget[ "volume" ];
    if( targetVolume >= phs_.size() )
        throw MASA_EXCEPTION( "Invalid target volume identifier in GetMaxDistanceForPH : "
                                  + boost::lexical_cast< std::string >( targetVolume ) );
    return launcherType_.GetPHModificator( firer, target ) * phs_[ targetVolume ].GetMaxYForX( ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMinDistanceForPH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMinDistanceForPH( double ph, const wrapper::View& compTarget ) const
{
    const std::size_t targetVolume = compTarget[ "volume" ];
    if( targetVolume >= phs_.size() )
        throw MASA_EXCEPTION( "Invalid target volume identifier in GetMinDistanceForPH : "
                                  + boost::lexical_cast< std::string >( targetVolume ) );
    return launcherType_.GetPHModificator() * phs_[ targetVolume ].GetMinYForX( ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMaxRangeToFire
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMaxRangeToFire( double ph ) const
{
    double range = 0;
    for( auto it = phs_.begin(); it != phs_.end(); ++it )
        range = std::max( range, launcherType_.GetPHModificator() * it->GetMaxYForX( ph ) );
    return range;
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMaxRange
// Created: MCO 2012-12-06
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMaxRange() const
{
    return maxRange_;
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMinDistanceForPH
// Created: MCO 2012-05-09
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMinDistanceForPH( const wrapper::View& firer, const wrapper::View& target, double ph, const wrapper::View& compTarget ) const
{
    const std::size_t targetVolume = compTarget[ "volume" ];
    if( targetVolume >= phs_.size() )
        throw MASA_EXCEPTION( "Invalid target volume identifier in GetMinDistanceForPH : "
                                  + boost::lexical_cast< std::string >( targetVolume ) );
    return launcherType_.GetPHModificator( firer, target ) * phs_[ targetVolume ].GetMinYForX( ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetPH
// Created: NLD 2004-10-05
// Modified: JVT 2004-11-03
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetPH( const wrapper::View& firer, const wrapper::View& target, std::size_t targetVolume ) const
{
    if( targetVolume >= phs_.size() )
        throw MASA_EXCEPTION( "Invalid target volume identifier in GetDangerosity : "
                                  + boost::lexical_cast< std::string >( targetVolume ) );
    double distance = GET_HOOK( GetDistance )( firer, target );
    const double modificator = launcherType_.GetPHModificator( firer, target );
    if( modificator <= 0 )
        return 0;
    distance /= modificator;
    const double ph = phs_[ targetVolume ]( distance );
    return dotation_.ModifyPh( firer, target, ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetPH
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetPH( std::size_t targetVolume, double distance ) const
{
    if( targetVolume >= phs_.size() )
        throw MASA_EXCEPTION( "Invalid target volume identifier in GetDangerosity : "
                                  + boost::lexical_cast< std::string >( targetVolume ) );
    const double modificator = launcherType_.GetPHModificator();
    if( modificator <= 0 )
        return 0;
    distance /= modificator;
    return phs_[ targetVolume ]( distance );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMaxRangeToFireOn( const wrapper::View& compTarget, double rWantedPH ) const
{
    return GetMaxDistanceForPH( rWantedPH, compTarget );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMinRangeToFireOn( const wrapper::View& compTarget, double rWantedPH ) const
{
    return GetMinDistanceForPH( rWantedPH, compTarget );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMaxRangeToFireOnWithPosture( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, double rWantedPH ) const
{
    return GetMaxDistanceForPH( firer, target, rWantedPH, compTarget );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMinRangeToFireOnWithPosture( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, double rWantedPH ) const
{
    return GetMinDistanceForPH( firer, target, rWantedPH, compTarget );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetDangerosity
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetDangerosity( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH ) const
{
    const double rDangerosity  = bUsePH ? GetPH( firer, target, compTarget[ "volume" ] ) : 1.;
    return rDangerosity * dotation_.ModifyDangerosity( compTarget );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetDangerosity( const wrapper::View& compTarget, double distance ) const
{
    return GetPH( compTarget[ "volume" ], distance ) * dotation_.ModifyDangerosity( compTarget );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::Fire
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void WeaponDataType_DirectFire::Fire( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH ) const
{
    const bool hit = ! bUsePH || (1 - GET_HOOK( GetFireRandomNumber )( 0, 1 )) <= GetPH( firer, target, compTarget[ "volume" ] );
    wrapper::Event event( "direct fire pion" );
    event[ "entity/identifier" ] = static_cast< std::size_t >( firer[ "identifier" ] );
    event[ "entity/data" ] = firer[ "data" ];
    event[ "enemy/data" ] = target[ "data" ];
    event[ "component/data" ] = compTarget[ "data" ];
    event[ "dotation" ] = dotation_.GetIdentifier();
    event[ "running" ] = true;
    event[ "hit" ] = hit;
    event[ "use-ph" ] = bUsePH;
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::Fire
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void WeaponDataType_DirectFire::Fire( const wrapper::View& firer, const wrapper::View& element, std::size_t ammos ) const
{
    const double ph = GET_HOOK( GetPopulationElementPh )( firer, element );
    std::size_t hits = 0;
    for( std::size_t i = 0; i < ammos; ++i )
        if( GET_HOOK( GetFireRandomNumber )( 0, 1 ) < ph )
            ++hits;
    wrapper::Event event( "direct fire population" );
    event[ "entity/identifier" ] = firer[ "identifier" ];
    event[ "entity/data" ] = firer[ "data" ];
    event[ "element" ] = element;
    event[ "hits" ] = hits;
    event[ "dotation" ] = dotation_.GetIdentifier();
    event[ "running" ] = true;
    event.Post();
}
