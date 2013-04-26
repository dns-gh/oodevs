// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "RoleAction_DirectFiring.h"
#include "simulation_kernel/MIL_Random.h"
#include <wrapper/Hook.h>
#include <wrapper/View.h>
#include <wrapper/Event.h>
#include <boost/bind.hpp>

using namespace sword;
using namespace sword::fire;

DECLARE_HOOK( CanComponentBeFiredAt, bool, ( const SWORD_Model* component, const SWORD_Model* parameters ) )
DECLARE_HOOK( GetFireRandomInteger, size_t, ( size_t min, size_t max ) )
DECLARE_HOOK( IsAgentKnowledgeValid, bool, ( const SWORD_Model* knowledge ) )
DECLARE_HOOK( IsPopulationKnowledgeValid, bool, ( const SWORD_Model* entity, const SWORD_Model* knowledge ) )
DECLARE_HOOK( GetClosestAlivePopulationElement, const SWORD_Model*, ( const SWORD_Model* model, const SWORD_Model* population, const SWORD_Model* entity ) )

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring constructor
// Created: MCO 2012-08-30
// -----------------------------------------------------------------------------
RoleAction_DirectFiring::RoleAction_DirectFiring( ModuleFacade& module )
    : module_( module )
{
    // NOTHING
}

namespace
{
    template< typename C >
    void random_shuffle( C& c )
    {
        auto func = boost::bind( GET_HOOK( GetFireRandomInteger ), 0u, _1 );
        std::random_shuffle( c.begin(), c.end(), func );
    }
}

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::GetComposantesAbleToBeFired
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
RoleAction_DirectFiring::T_Components RoleAction_DirectFiring::GetComposantesAbleToBeFired(
    const wrapper::View& components, const wrapper::View& parameters, std::size_t nNbrWeaponsUsable ) const
{
    T_Components availableTargets;
    for( std::size_t c = 0; c < components.GetSize(); ++c )
    {
        const wrapper::View& fired = components.GetElement( c );
        if( GET_HOOK( CanComponentBeFiredAt )( fired, parameters ) )
            availableTargets.push_back( fired );
    }
    T_Components targets;
    if( availableTargets.empty() )
        return targets;
    while( targets.size() < nNbrWeaponsUsable )
    {
        random_shuffle( availableTargets );
        std::copy( availableTargets.begin(), availableTargets.end(), std::back_inserter( targets ) );
    }
    targets.resize( nNbrWeaponsUsable );
    return targets;
}

namespace
{
    void NotifyAttacking( const wrapper::View& entity, const wrapper::View& target, bool mustReport, bool paused )
    {
        wrapper::Event event( "direct fire pion attack" );
        event[ "entity/data" ] = entity[ "data" ];
        event[ "enemy/data" ] = target[ "data" ];
        event[ "report" ] = mustReport;
        event[ "paused" ] = paused;
        event.Post();
    }
}

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::FirePion
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
int RoleAction_DirectFiring::FirePion( const wrapper::View& model, const wrapper::View& entity,
    const wrapper::View& target, const wrapper::View& parameters, bool mustReport ) const
{
    if( ! GET_HOOK( IsAgentKnowledgeValid )( target ) )
        return eImpossible;
    if( target[ "is-dead" ] )
        return eEnemyDestroyed;
    // Firers
    DirectFireData data( module_, entity, parameters );
    if( data.CanFire( entity ) )
    {
        const wrapper::View& components = entity[ "components" ];
        for( std::size_t c = 0; c < components.GetSize(); ++c )
        {
            const wrapper::View& component = components.GetElement( c );
            const wrapper::View& weapons = component[ "weapons" ];
            for( std::size_t w = 0; w < weapons.GetSize(); ++w )
                data.ApplyOnWeapon( model, component, weapons.GetElement( w ) );
        }
    }
    const std::size_t nNbrWeaponsUsable = data.GetUsableWeapons();
    if( nNbrWeaponsUsable == 0 )
    {
        if( data.HasWeaponsNotReady() )
            return eRunning;
        if( data.HasWeaponsAndNoAmmo() )
            return eNoAmmo;
        if( data.IsTemporarilyBlocked() )
            return eTemporarilyBlocked;
        return eNoCapacity;
    }
    const wrapper::View& enemy = model[ "entities" ][ static_cast< unsigned int >( target[ "identifier" ] ) ];
    const wrapper::View& targets = enemy[ "components" ];
    T_Components compTargets = GetComposantesAbleToBeFired( targets, parameters, nNbrWeaponsUsable );
    if( compTargets.empty() )
        return eEnemyDestroyed;
    NotifyAttacking( entity, enemy, mustReport, false );
    assert( compTargets.size() == nNbrWeaponsUsable );
    data.Fire( enemy, compTargets );
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::FirePionSuspended
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void RoleAction_DirectFiring::FirePionSuspended( const wrapper::View& model, const wrapper::View& entity, const wrapper::View& target, bool mustReport ) const
{
    if( GET_HOOK( IsAgentKnowledgeValid )( target ) )
    {
        const wrapper::View& enemy = model[ "entities" ][ static_cast< unsigned int >( target[ "identifier" ] ) ];
        NotifyAttacking( entity, enemy, mustReport, true );
    }
}

//// -----------------------------------------------------------------------------
//// Name: RoleAction_DirectFiring::IlluminatePion
//// Created: MGD 2010-02-12
//// -----------------------------------------------------------------------------
//int  RoleAction_DirectFiring::IlluminatePion( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy )
//{
//    MIL_Agent_ABC* pTarget = pEnemy && pEnemy->IsValid() ? &pEnemy->GetAgentKnown() : 0;
//    if( !pTarget )
//        return eImpossible;
//    if( pTarget->IsDead() )
//        return eEnemyDestroyed;
//    if( pTarget->GetRole< PHY_RoleInterface_Illumination >().IsUnderIndirectFire() )
//        return eFinished;
//    const dotation::PHY_RoleInterface_Dotations& roleDotations = owner_.GetRole< dotation::PHY_RoleInterface_Dotations >();
//    const PHY_RoleInterface_Location& pionLocation = owner_.Get< PHY_RoleInterface_Location >();
//    double range = pionLocation.GetPosition().Distance( pEnemy->GetPosition() );
//    const PHY_DotationCategory* munition = roleDotations.GetIlluminationDotations( (float)range , true );
//    if( munition  )
//    {
//        pTarget->GetRole< PHY_RoleInterface_Illumination >().NotifyDefinitelyIlluminated();
//        double consommation = 1.;
//        owner_.Apply( &dotation::ConsumeDotationNotificationHandler_ABC::NotifyConsumeDotation, *munition, consommation );
//        return eFinished;
//    }
//    munition = roleDotations.GetIlluminationDotations( (float)range, false );
//    if( munition )
//    {
//        pTarget->GetRole< PHY_RoleInterface_Illumination >().NotifyStartIlluminatedBy( owner_ );
//        owner_.GetRole< PHY_RoleInterface_Illumination >().NotifyStartIlluminate( *pTarget );
//        return eRunning;
//    }
//    else
//        return eNoCapacity;
//}
//// -----------------------------------------------------------------------------
//// Name: RoleAction_DirectFiring::IlluminatePionSuspended
//// Created: MGD 2010-02-12
//// -----------------------------------------------------------------------------
//void RoleAction_DirectFiring::IlluminatePionSuspended( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy )
//{
//    MIL_Agent_ABC* pTarget = pEnemy && pEnemy->IsValid() ? &pEnemy->GetAgentKnown() : 0;
//    if( !pTarget )
//    {
//        pTarget->GetRole< PHY_RoleInterface_Illumination >().NotifyStopIlluminatedBy( owner_ );
//        owner_.GetRole< PHY_RoleInterface_Illumination >().NotifyStopIlluminate();
//    }
//}
//
//// -----------------------------------------------------------------------------
//// Name: RoleAction_DirectFiring::FireZone
//// Created: NLD 2004-10-27
//// -----------------------------------------------------------------------------
//void RoleAction_DirectFiring::FireZone( const MIL_Object_ABC& object, FireResults_Default*& pFireResult )
//{
//    if( !pFireResult )
//        pFireResult = new FireResults_Default();
//    typedef std::pair< MIL_Agent_ABC*, PHY_Composante_ABC* >   T_TargetPair;
//    typedef std::vector< T_TargetPair >                        T_TargetVector;
//    typedef T_TargetVector::const_iterator                   CIT_TargetVector;
//    T_TargetVector targets;
//    const ControlZoneCapacity* capacity = object.Retrieve< ControlZoneCapacity >();
//    if( capacity )
//        capacity->RetrieveTargets( object, targets );
//    DirectFireData data( owner_, DirectFireData::eFireUsingOnlyComposantesLoadable, DirectFireData::eFiringModeNormal );
//    std::auto_ptr< WeaponAvailabilityComputer_ABC > weaponAvailabilityComputer( owner_.GetAlgorithms().weaponAvailabilityComputerFactory_->Create( data ) );
//    owner_.Execute( *weaponAvailabilityComputer );
//    for( CIT_TargetVector itTarget = targets.begin(); itTarget != targets.end(); ++itTarget )
//    {
//        MIL_Agent_ABC& target = *itTarget->first;
//        PHY_Composante_ABC& compTarget = *itTarget->second;
//        const PHY_ComposantePion* pCompFirer = 0;
//        Weapon*  pFirerWeapon = 0;
//        data.ChooseRandomWeapon( target, compTarget, pCompFirer, pFirerWeapon );
//        if( !pCompFirer )
//            continue;
//        assert( pFirerWeapon );
//        pFirerWeapon->DirectFire( owner_, *itTarget->first, *itTarget->second, *pFireResult, false ); // 'false' is for 'don't use ph'
//        data.ReleaseWeapon( *pCompFirer, *pFirerWeapon );
//    }
//}

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::FirePopulation
//// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
int RoleAction_DirectFiring::FirePopulation( const wrapper::View& model, const wrapper::View& entity,
    const wrapper::View& target, const wrapper::View& parameters ) const
{
    if( ! GET_HOOK( IsPopulationKnowledgeValid )( entity, target ) )
        return eImpossible;
    const SWORD_Model* element = GET_HOOK( GetClosestAlivePopulationElement )( model, target, entity );
    if( ! element )
        return eEnemyDestroyed;
    // Firers
    DirectFireData data( module_, entity, parameters );
    if( data.CanFire( entity ) )
    {
        const wrapper::View& components = entity[ "components" ];
        for( std::size_t c = 0; c < components.GetSize(); ++c )
        {
            const wrapper::View& component = components.GetElement( c );
            const wrapper::View& weapons = component[ "weapons" ];
            for( std::size_t w = 0; w < weapons.GetSize(); ++w )
                data.ApplyOnWeapon( model, component, weapons.GetElement( w ) );
        }
    }
    const std::size_t nNbrWeaponsUsable = data.GetUsableWeapons();
    if( nNbrWeaponsUsable == 0 )
    {
        if( data.HasWeaponsNotReady() )
            return eRunning;
        if( data.HasWeaponsAndNoAmmo() )
            return eNoAmmo;
        if( data.IsTemporarilyBlocked() )
            return eTemporarilyBlocked;
        return eNoCapacity;
    }
    {
        wrapper::Event event( "direct fire population attack" );
        event[ "entity" ] = entity;
        event[ "population" ] = target;
        event[ "element" ] = element;
        event[ "paused" ] = false;
        event.Post();
    }
    // Tir
    data.Fire( element );
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::FirePopulationSuspended
// Created: MCO 2012-09-17
// -----------------------------------------------------------------------------
void RoleAction_DirectFiring::FirePopulationSuspended( const wrapper::View& entity, const wrapper::View& target ) const
{
    if( GET_HOOK( IsPopulationKnowledgeValid )( entity, target ) )
    {
        wrapper::Event event( "direct fire population attack" );
        event[ "entity" ] = entity;
        event[ "population" ] = target;
        event[ "paused" ] = true;
        event.Post();
    }
}

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::GetInitialReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
int RoleAction_DirectFiring::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::GetFinalReturnCode
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
int RoleAction_DirectFiring::GetFinalReturnCode() const
{
    return eFinished;
}