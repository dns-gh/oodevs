// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "Facade.h"
#include "Agent.h"
#include "ReflectedCreationListener_ABC.h"
#pragma warning( push, 0 )
#include <vl/reflAggList.h>
#include <vl/reflEntList.h>
#include <vrforces/vrfBeListener.h>
#include <vrforces/vrfController.h>
#pragma warning( pop )

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
Facade::Facade( DtExerciseConn& connection )
    : controller_( createVrfRemoteController() )
{
    if( !controller_.get() )
        throw std::runtime_error( __FUNCTION__ ": unable to create Vrf Remote Controller" );
    controller_->init( &connection );
    controller_->backendListener()->addBackendDiscoveryCallback( &Facade::OnBackendDiscovery, this );
    controller_->backendListener()->addBackendRemovalCallback( &Facade::OnBackendRemoval, this );
    reflectedAggregates_.reset( new DtReflectedAggregateList( &connection ) );
    reflectedAggregates_->addAggregateAdditionCallback( &Facade::OnAggregateAddition, this );
    reflectedAggregates_->addAggregateRemovalCallback( &Facade::OnAggregateRemoval, this );
    reflectedAggregates_->setDiscoveryCondition( &Facade::CheckDiscoveryCondition, 0 );
    reflectedEntities_.reset( new DtReflectedEntityList( &connection ) );
    reflectedEntities_->addEntityAdditionCallback( &Facade::OnEntityAddition, this );
    reflectedEntities_->addEntityRemovalCallback( &Facade::OnEntityRemoval, this );
    reflectedEntities_->setDiscoveryCondition( &Facade::CheckDiscoveryCondition, 0 );
}

// -----------------------------------------------------------------------------
// Name: Facade destructor
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
Facade::~Facade()
{
    reflectedEntities_->removeEntityRemovalCallback( &Facade::OnEntityRemoval, this );
    reflectedEntities_->removeEntityAdditionCallback( &Facade::OnEntityAddition, this );
    reflectedAggregates_->removeAggregateRemovalCallback( &Facade::OnAggregateRemoval, this );
    reflectedAggregates_->removeAggregateAdditionCallback( &Facade::OnAggregateAddition, this );
    controller_->backendListener()->removeBackendRemovalCallback( &Facade::OnBackendRemoval, this );
    controller_->backendListener()->removeBackendDiscoveryCallback( &Facade::OnBackendDiscovery, this );
}

// -----------------------------------------------------------------------------
// Name: Facade::OnBackendDiscovery
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
void Facade::OnBackendDiscovery( const DtSimulationAddress& addr, void* usr )
{
    if( Facade* that = static_cast< Facade* >( usr ) )
        that->StartScenario( addr );
}

// -----------------------------------------------------------------------------
// Name: Facade::OnBackendRemoval
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::OnBackendRemoval( const DtSimulationAddress& addr, void* usr )
{
    if( Facade* that = static_cast< Facade* >( usr ) )
        that->address_ = DtSimulationAddress::nullId();
}

// -----------------------------------------------------------------------------
// Name: Facade::StartScenario
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::StartScenario( const DtSimulationAddress& address )
{
    address_ = address;
    std::vector< DtString > simulationModelFiles;
    simulationModelFiles.push_back( "../data/simulationModelSets/default.sms" );
    controller_->newScenario( "../data/terrain/WorldFlatEarth.mtd", "../data/terrain/WorldFlatEarth.mtd", simulationModelFiles );
}

// -----------------------------------------------------------------------------
// Name: Facade::CreatePseudoAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::CreatePseudoAggregate( Agent& agent )
{
    if( IsConnected() )
        agent.CreatePseudoAggregate( *controller_, address_ );
}

// -----------------------------------------------------------------------------
// Name: Facade::DestroyPseudoAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::DestroyPseudoAggregate( const DtEntityIdentifier& reflected )
{
    if( IsConnected() )
        controller_->deleteObject( reflected, address_ );
}

// -----------------------------------------------------------------------------
// Name: Facade::OnAggregateAddition
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::OnAggregateAddition( DtReflectedAggregate* obj, void* userData )
{
    if( Facade* that = static_cast< Facade* >( userData ) )
    {
        that->addedAggregates_[ obj->asr()->entityId().string() ] = obj;
        for( T_ReflectedCreationListeners::iterator it = that->listeners_.begin(); it != that->listeners_.end(); )
            if( (*it)->NotifyCreated( *obj ) )
                it = that->listeners_.erase( it );
            else
                ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::OnAggregateRemoval
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::OnAggregateRemoval( DtReflectedAggregate* obj, void* userData )
{
    if( Facade* that = static_cast< Facade* >( userData ) )
    {
        T_AggregateResolver::iterator it = that->addedAggregates_.find( obj->asr()->entityId().string() );
        that->addedAggregates_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::OnEntityAddition
// Created: SBO 2011-04-01
// -----------------------------------------------------------------------------
void Facade::OnEntityAddition( DtReflectedEntity* obj, void* userData )
{
    if( Facade* that = static_cast< Facade* >( userData ) )
    {
        that->addedEntities_[ obj->esr()->entityId().string() ] = obj;
        for( T_ReflectedCreationListeners::iterator it = that->listeners_.begin(); it != that->listeners_.end(); )
            if( (*it)->NotifyCreated( *obj ) )
                it = that->listeners_.erase( it );
            else
                ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::OnEntityRemoval
// Created: SBO 2011-04-01
// -----------------------------------------------------------------------------
void Facade::OnEntityRemoval( DtReflectedEntity* obj, void* userData )
{
    if( Facade* that = static_cast< Facade* >( userData ) )
    {
        T_EntityResolver::iterator it = that->addedEntities_.find( obj->esr()->entityId().string() );
        that->addedEntities_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::CheckDiscoveryCondition
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
bool Facade::CheckDiscoveryCondition( DtReflectedObject* reflected, void* usr )
{
    if( DtReflectedAggregate* aggregate = dynamic_cast< DtReflectedAggregate* >( reflected ) )
        return aggregate->asr()->entityId() != DtEntityIdentifier::nullId();
    if( DtReflectedEntity* entity = dynamic_cast< DtReflectedEntity* >( reflected ) )
        return entity->esr()->entityId() != DtEntityIdentifier::nullId();
    return false;
}

// -----------------------------------------------------------------------------
// Name: Facade::Lookup
// Created: SBO 2011-04-01
// -----------------------------------------------------------------------------
void Facade::Lookup( const DtEntityIdentifier& id, ReflectedCreationListener_ABC& listener )
{
    {
        T_AggregateResolver::const_iterator it = addedAggregates_.find( id.string() );
        if( it != addedAggregates_.end() )
            listener.NotifyCreated( *it->second );
    }
    {
        T_EntityResolver::const_iterator it = addedEntities_.find( id.string() );
        if( it != addedEntities_.end() )
            listener.NotifyCreated( *it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::IsConnected
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
bool Facade::IsConnected() const
{
    return address_ != DtSimulationAddress::nullId();
}

// -----------------------------------------------------------------------------
// Name: Facade::AddListener
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::AddListener( ReflectedCreationListener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: Facade::RemoveListener
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::RemoveListener( ReflectedCreationListener_ABC& listener )
{
    T_ReflectedCreationListeners::iterator it = std::find( listeners_.begin(), listeners_.end(), &listener );
    if( it != listeners_.end() )
        listeners_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: Facade::CreateDisaggregationArea
// Created: SBO 2011-03-22
// -----------------------------------------------------------------------------
void Facade::CreateDisaggregationArea( const DtString& name, const DtList& vertices ) const
{
    if( IsConnected() )
        controller_->createDisaggregationArea( vertices, name, name, address_ );
}
