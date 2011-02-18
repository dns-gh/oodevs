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
#pragma warning( push, 0 )
#include <vl/reflAggList.h>
#include <vrforces/vrfBeListener.h>
#include <vrforces/vrfController.h>
#pragma warning( pop )
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
Facade::Facade( DtExerciseConn& connection )
    : controller_( createVrfRemoteController() )
{
    controller_->init( &connection );
    reflectedAggregates_.reset( new DtReflectedAggregateList( &connection ) );
//    boost::function< void ( const DtSimulationAddress& addr, void* usr ) > function = boost::bind( &Facade::OnBackendDiscovery, this, _1, _2 );
//    controller_->backendListener()->addBackendDiscoveryCallback( *function.target< DtBackendCallbackFcn >(), 0 );
    controller_->backendListener()->addBackendDiscoveryCallback( &Facade::OnBackendDiscovery, this );
    controller_->backendListener()->addBackendRemovalCallback( &Facade::OnBackendRemoval, this );
    reflectedAggregates_->addAggregateAdditionCallback( &Facade::OnAggregateAddition, this );
    reflectedAggregates_->addAggregateRemovalCallback( &Facade::OnAggregateRemoval, this );
    reflectedAggregates_->setDiscoveryCondition( &Facade::CheckAggregateDiscoveryCondition, 0 );
}

// -----------------------------------------------------------------------------
// Name: Facade destructor
// Created: SBO 2011-01-20
// -----------------------------------------------------------------------------
Facade::~Facade()
{
    // NOTHING
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
        that->reflected_[ obj->aggregateStateRep()->entityId().string() ] = obj;
        for( T_ReflectedCreationListeners::iterator it = that->reflectedCreationListeners_.begin(); it != that->reflectedCreationListeners_.end(); )
        {
            if( (*it)->OnCreateReflected( obj ) )
            {
                T_ReflectedCreationListeners::iterator itDelete = it;
                ++it;
                that->reflectedCreationListeners_.erase( itDelete );
            }
            else
                ++it;
        }
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
        T_Reflected::iterator it = that->reflected_.find( obj->aggregateStateRep()->entityId().string() );
        that->reflected_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::CheckAggregateDiscoveryCondition
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
bool Facade::CheckAggregateDiscoveryCondition( DtReflectedObject* refObj, void* usr )
{
    if( DtReflectedAggregate* aggregate = static_cast< DtReflectedAggregate* >( refObj ) )
        return aggregate->asr()->entityId() != DtEntityIdentifier::nullId();
    return false;
}

// -----------------------------------------------------------------------------
// Name: Facade::Find
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
const DtReflectedAggregate* Facade::Find( const DtEntityIdentifier& id ) const
{
    T_Reflected::const_iterator it = reflected_.find( id.string() );
    if( it != reflected_.end() )
        return it->second;
    return 0;
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
// Name: Facade::RegisterReflectedCreationListener
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::RegisterReflectedCreationListener( Agent& agent )
{
    reflectedCreationListeners_.push_back( &agent );
}

// -----------------------------------------------------------------------------
// Name: Facade::UnregisterReflectedCreationListener
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Facade::UnregisterReflectedCreationListener( Agent& agent )
{
    T_ReflectedCreationListeners::iterator it = std::find( reflectedCreationListeners_.begin(), reflectedCreationListeners_.end(), &agent );
    reflectedCreationListeners_.erase( it );
}
