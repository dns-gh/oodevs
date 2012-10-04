// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "RoleAction_DirectFiring.h"
#include "Weapon.h"
#include "simulation_kernel/MIL_Random.h"
#include <wrapper/Hook.h>
#include <wrapper/View.h>
#include <wrapper/Event.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <iterator>

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

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::FirePion
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void RoleAction_DirectFiring::FirePion( DirectFireData& data, const wrapper::View& entity, const wrapper::View& target, const T_ComposanteVector& compTargets ) const
{
    // Pour chaque cible, choix de la meilleure arme
    unsigned int nNbrWeaponsUsed = 0;
    for( CIT_ComposanteVector it = compTargets.begin(); it != compTargets.end(); ++it )
    {
        const wrapper::View& compTarget = *it;
        const SWORD_Model* pBestFirer = 0;
        const Weapon* pBestFirerWeapon = 0;
        data.ChooseBestWeapon( target, compTarget, pBestFirer, pBestFirerWeapon );
        if( !pBestFirer )
            continue;
        assert( pBestFirerWeapon );
        pBestFirerWeapon->DirectFire( entity, target, compTarget, true ); // 'true' is for 'use ph'
        ++nNbrWeaponsUsed;
        data.ReleaseWeapon( pBestFirer, *pBestFirerWeapon );
    }
    // Pour toutes les armes non utilisées, choix de la meilleure cible
    const SWORD_Model* pUnusedFirer = 0;
    const Weapon* pUnusedFirerWeapon = 0;
    while( data.GetUnusedFirerWeapon( pUnusedFirer, pUnusedFirerWeapon ) && nNbrWeaponsUsed < compTargets.size() ) // $$$$ MCO 2012-08-30: not sure why we stop firing when nNbrWeaponsUsed reaches compTargets.size()
    {
        const wrapper::View* pBestCompTarget = 0;
        double rBestScore = 0;
        for( CIT_ComposanteVector it = compTargets.begin(); it != compTargets.end(); ++it )
        {
            const wrapper::View& compTarget = *it;
            const double rCurrentScore = pUnusedFirerWeapon->GetDangerosity( entity, target, compTarget, true, true ); // 'true' is for 'use ph' and true for 'use ammo'
            if( rCurrentScore > rBestScore )
            {
                rBestScore = rCurrentScore;
                pBestCompTarget = &compTarget;
            }
        }
        if( pBestCompTarget )
        {
            pUnusedFirerWeapon->DirectFire( entity, target, *pBestCompTarget, true ); // 'true' is for 'use ph'
            ++nNbrWeaponsUsed;
        }
        data.ReleaseWeapon( pUnusedFirer, *pUnusedFirerWeapon );
    }
}

namespace
{
    template< typename C >
    void random_shuffle( C& c )
    {
        boost::function< std::size_t( std::size_t ) > func = boost::bind( GET_HOOK( GetFireRandomInteger ), 0u, _1 );
        std::random_shuffle( c.begin(), c.end(), func );
    }
}

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::GetComposantesAbleToBeFired
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
RoleAction_DirectFiring::T_ComposanteVector RoleAction_DirectFiring::GetComposantesAbleToBeFired( const wrapper::View& components, const wrapper::View& parameters, unsigned int nNbrWeaponsUsable ) const
{
    T_ComposanteVector availableTargets;
    for( std::size_t c = 0; c < components.GetSize(); ++c )
    {
        const wrapper::View& fired = components.GetElement( c );
        if( GET_HOOK( CanComponentBeFiredAt )( fired, parameters ) )
            availableTargets.push_back( fired );
    }
    if( availableTargets.empty() )
        return availableTargets;
    T_ComposanteVector targets;
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
        event[ "entity" ] = static_cast< std::size_t >( entity[ "identifier" ] );
        event[ "enemy" ] = static_cast< std::size_t >( target[ "identifier" ] );
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
    if( target[ "dead" ] )
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
    const unsigned int nNbrWeaponsUsable = data.GetNbrWeaponsUsable();
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
    NotifyAttacking( entity, target, mustReport, false );
    const wrapper::View& targets = model[ "entities" ][ static_cast< unsigned int >( target[ "identifier" ] ) ][ "components" ];
    T_ComposanteVector compTargets = GetComposantesAbleToBeFired( targets, parameters, nNbrWeaponsUsable );
    if( compTargets.empty() )
        return eEnemyDestroyed;
    assert( compTargets.size() == nNbrWeaponsUsable );
    FirePion( data, entity, target, compTargets );
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_DirectFiring::FirePionSuspended
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void RoleAction_DirectFiring::FirePionSuspended( const wrapper::View& entity, const wrapper::View& target, bool mustReport ) const
{
    if( GET_HOOK( IsAgentKnowledgeValid )( target ) )
        NotifyAttacking( entity, target, mustReport, true );
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
    const unsigned int nNbrWeaponsUsable = data.GetNbrWeaponsUsable();
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
    const SWORD_Model* pFirer = 0;
    const Weapon* pFirerWeapon = 0;
    while( data.GetUnusedFirerWeapon( pFirer, pFirerWeapon ) )
    {
        pFirerWeapon->DirectFire( entity, element );
        data.ReleaseWeapon( pFirer, *pFirerWeapon );
    }
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
