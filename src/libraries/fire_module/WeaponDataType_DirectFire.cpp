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
#include <xeumeuleu/xml.hpp>
#include <wrapper/Hook.h>
#include <wrapper/View.h>
#include <wrapper/Event.h>
#include <module_api/Log.h>
#include <boost/lexical_cast.hpp>
#include <cassert>

using namespace sword;
using namespace sword::fire;

DECLARE_HOOK( GetVolumeId, unsigned int, ( const char* type ) )
DECLARE_HOOK( GetDistance, double, ( const SWORD_Model* firer, const SWORD_Model* target ) )
DECLARE_HOOK( GetFireRandomNumber, double, ( double min, double max ) )

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
WeaponDataType_DirectFire::WeaponDataType_DirectFire( const LauncherType& launcherType, const DotationCategory& dotation, xml::xistream& xis )
    : launcherType_( launcherType )
    , dotation_    ( dotation )
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
    unsigned int id = GET_HOOK( GetVolumeId )( targetType.c_str() );
    if( phs_.size() <= id )
        phs_.resize( id + 1 );
    MT_InterpolatedFunction< double >& phFunction = phs_[ id ]; // $$$$ MCO 2012-04-30: how to handle unknown type ?
    xis >> xml::list( "hit-probability", *this, &WeaponDataType_DirectFire::ReadHitProbability, phFunction );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::ReadHitProbability
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void WeaponDataType_DirectFire::ReadHitProbability( xml::xistream& xis, MT_InterpolatedFunction< double >& phFunction )
{
    double rDistance;
    double ph;
    xis >> xml::attribute( "distance", rDistance )
        >> xml::attribute( "percentage", ph );
    if( rDistance < 0 )
        xis.error( "hit-probability: distance <  0" );
    if( ph < 0 || ph > 1 )
        xis.error( "hit-probability: percentage not in [0.1]" );
    phFunction.AddNewPoint( rDistance, ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMaxDistanceForPH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMaxDistanceForPH( double ph, const wrapper::View& compTarget ) const
{
    const std::size_t targetVolume = compTarget[ "volume" ];
    if( targetVolume >= phs_.size() )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR,
            ( "Invalid target volume identifier in GetMaxDistanceForPH : "
                + boost::lexical_cast< std::string >( targetVolume ) ).c_str() );
        return 0;
    }
    return launcherType_.GetPHModificator()
        * phs_[ targetVolume ].GetMaxYForX( ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMaxDistanceForPH
// Created: MCO 2012-05-09
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMaxDistanceForPH( const wrapper::View& firer, const wrapper::View& target, double ph, const wrapper::View& compTarget ) const
{
    const std::size_t targetVolume = compTarget[ "volume" ];
    if( targetVolume >= phs_.size() )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR,
            ( "Invalid target volume identifier in GetMaxDistanceForPH : "
                + boost::lexical_cast< std::string >( targetVolume ) ).c_str() );
        return 0;
    }
    return launcherType_.GetPHModificator( firer, target )
        * phs_[ targetVolume ].GetMaxYForX( ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMinDistanceForPH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMinDistanceForPH( double ph, const wrapper::View& compTarget ) const
{
    const std::size_t targetVolume = compTarget[ "volume" ];
    if( targetVolume >= phs_.size() )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR,
            ( "Invalid target volume identifier in GetMinDistanceForPH : "
                + boost::lexical_cast< std::string >( targetVolume ) ).c_str() );
        return std::numeric_limits< double >::max();
    }
    return launcherType_.GetPHModificator()
        * phs_[ targetVolume ].GetMinYForX( ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMaxRangeToFire
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMaxRangeToFire( double ph ) const
{
    double range = 0;
    for( CIT_PhVector it = phs_.begin(); it != phs_.end(); ++it )
        range = std::max( range, launcherType_.GetPHModificator() * it->GetMaxYForX( ph ) );
    return range;
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetMinDistanceForPH
// Created: MCO 2012-05-09
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetMinDistanceForPH( const wrapper::View& firer, const wrapper::View& target, double ph, const wrapper::View& compTarget ) const
{
    const std::size_t targetVolume = compTarget[ "volume" ];
    if( targetVolume >= phs_.size() )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR,
            ( "Invalid target volume identifier in GetMinDistanceForPH : "
                + boost::lexical_cast< std::string >( targetVolume ) ).c_str() );
        return std::numeric_limits< double >::max();
    }
    return launcherType_.GetPHModificator( firer, target )
        * phs_[ targetVolume ].GetMinYForX( ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetPH
// Created: NLD 2004-10-05
// Modified: JVT 2004-11-03
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetPH( const wrapper::View& firer, const wrapper::View& target, std::size_t targetVolume ) const
{
    double rDistance = GET_HOOK( GetDistance )( firer, target );
    const double rPHModificator = launcherType_.GetPHModificator( firer, target );
    if( rPHModificator <= 0 )
        return 0;
    rDistance /= rPHModificator;
    if( phs_.size() <= targetVolume )
        return MT_InterpolatedFunction< double >()( rDistance );
    const double ph = phs_[ targetVolume ]( rDistance );
    return dotation_.ModifyPh( firer, target, ph );
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_DirectFire::GetPH
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double WeaponDataType_DirectFire::GetPH( std::size_t targetVolume, double rDistance ) const
{
    if( targetVolume >= phs_.size() )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR,
            ( "Invalid target volume identifier in GetDangerosity : "
                + boost::lexical_cast< std::string >( targetVolume ) ).c_str() );
        return 0;
    }
    const double rPHModificator = launcherType_.GetPHModificator();
    if( rPHModificator <= 0 )
        return 0;
    rDistance /= rPHModificator;
    return phs_[ targetVolume ]( rDistance );
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
    const bool missed = bUsePH ? GET_HOOK( GetFireRandomNumber )( 0, 1 ) < GetPH( firer, target, compTarget[ "volume" ] ) : false;
    wrapper::Event event( "direct fire pion" );
    event[ "entity" ] = static_cast< std::size_t >( firer[ "identifier" ] );
    event[ "enemy" ] = static_cast< std::size_t >( target[ "identifier" ] );
    event[ "component" ].SetUserData( compTarget[ "component" ].GetUserData() );
    event[ "dotation" ] = dotation_.GetName();
    event[ "running" ] = true;
    event[ "missed" ] = missed;
    event[ "use-ph" ] = bUsePH;
    event.Post();

    // $$$$ MCO 2012-04-26: need to handle collateral damage on population
    //const PHY_RoleInterface_Location& firerLocation = firer.GetRole< PHY_RoleInterface_Location >();
    //const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();

    // handle direct-indirect fire on populations
    //const MT_Vector2D firerPosition ( firerLocation.GetPosition().rX_, firerLocation.GetPosition().rY_ );
    //const MT_Vector2D targetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_ );

    //TER_PopulationConcentration_ABC::T_PopulationConcentrationVector concentrations;
    //TER_World::GetWorld().GetPopulationManager().GetConcentrationManager()
    //                     .GetListIntersectingLine( firerPosition, targetPosition, concentrations );
    //for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector itConcentration = concentrations.begin();
    //    itConcentration != concentrations.end(); ++itConcentration )
    //{
    //    MIL_PopulationConcentration* pElement = static_cast< MIL_PopulationConcentration* >( *itConcentration );
    //    MIL_Effect_DirectFirePopulation* pEffect = new MIL_Effect_DirectFirePopulation( *pElement, 1, fireResult );
    //    MIL_EffectManager::GetEffectManager().Register( *pEffect );
    //}

    //TER_PopulationFlow_ABC::T_PopulationFlowVector flows;
    //TER_World::GetWorld().GetPopulationManager().GetFlowManager()
    //                     .GetListIntersectingLine( firerPosition, targetPosition, flows );
    //for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector itFlow = flows.begin(); itFlow != flows.end(); ++itFlow )
    //{
    //    MIL_PopulationFlow* pElement = static_cast< MIL_PopulationFlow* >( *itFlow );
    //    MIL_Effect_DirectFirePopulation* pEffect = new MIL_Effect_DirectFirePopulation( *pElement, 1, fireResult );
    //    MIL_EffectManager::GetEffectManager().Register( *pEffect );
    //}
}

//// -----------------------------------------------------------------------------
//// Name: WeaponDataType_DirectFire::Fire
//// Created: NLD 2005-11-16
//// -----------------------------------------------------------------------------
//void WeaponDataType_DirectFire::Fire( MIL_Agent_ABC& firer, MIL_PopulationElement_ABC& target, unsigned int nNbrAmmoReserved, PHY_FireResults_ABC& fireResult, const PHY_AmmoDotationClass* dotationClass ) const
//{
//    const PHY_RoePopulation& roe = firer.GetRole< DEC_RolePion_Decision >().GetRoePopulation();
//    const double ph = target.GetPopulation().GetType().GetDamagePH( roe );
//
//    unsigned int nHit = 0;
//    for( unsigned int i = 1; i <= nNbrAmmoReserved; ++i )
//        if( 1. - MIL_Random::rand_io() <= ph )
//            ++nHit;
//
//    MIL_Effect_ABC* pEffect = 0;
//    if( &PHY_AmmoDotationClass::alr_ == dotationClass )
//        pEffect = new MIL_Effect_ScatterPopulation( target, nHit, fireResult );
//    else
//        pEffect = new MIL_Effect_DirectFirePopulation( target, nHit, fireResult );
//    MIL_EffectManager::GetEffectManager().Register( *pEffect );
//}
