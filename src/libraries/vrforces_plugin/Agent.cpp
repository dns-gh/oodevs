// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "Agent.h"
#include "DisaggregationStrategy_ABC.h"
#include "Facade.h"
#include "ForceResolver_ABC.h"
#include "Subordinate.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "protocol/Protocol.h"
#include <boost/lexical_cast.hpp>
#pragma warning( push, 0 )
#include <matrix/geodcoord.h>
#include <matrix/topocoord.h>
#include <vl/aggPub.h>
#include <vl/aggregateSR.h>
#include <vl/reflectedAgg.h>
#include <vlpi/entitytypes.h>
#include <vrforces/vrfController.h>
#pragma warning( pop )

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
Agent::Agent( const kernel::Agent_ABC& agent, DtExerciseConn& connection, Facade& vrForces, const sword::UnitCreation& message, const ForceResolver_ABC& forces, const DisaggregationStrategy_ABC& disaggregation )
    : disaggregation_( disaggregation )
    , connection_    ( connection )
    , vrForces_      ( vrForces )
    , id_            ( message.unit().id() )
    , heading_       ( 0 )
    , reflectedId_   ( DtEntityIdentifier::nullId() )
    , reflected_     ( 0 )
    , type_          ( agent.GetType() )
{
    std::stringstream name;
    name << message.automat().id() << ":"<< id_ << "/" << message.name().c_str();
    DtEntityType type( DtPlatform, DtPlatformDomainLand, DtFrance, 3 /* platoon */, 2 /* armor */, message.pc(), 0 );
    publisher_.reset( new DtAggregatePublisher( type, &connection_, DtDrStatic, forces.Resolve( id_ ), name.str().c_str() ) );
    publisher_->asr()->setMarkingText( name.str().c_str() );
    CreateSubordinates( type_ );
    publisher_->tick();
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::GetId
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
unsigned long Agent::GetId() const
{
    return id_;
}

namespace
{
    unsigned int CountSubordinates( const kernel::AgentType& type )
    {
        int count = 0;
        for( tools::Iterator< const kernel::AgentComposition& > it = type.CreateIterator(); it.HasMoreElements(); it.NextElement() )
            ++count;
        return count;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateSubordinates
// Created: SBO 2011-03-22
// -----------------------------------------------------------------------------
void Agent::CreateSubordinates( const kernel::AgentType& type )
{
    publisher_->asr()->setNumberOfSilentEntities( CountSubordinates( type ) );
    tools::Iterator< const kernel::AgentComposition& > it = type.CreateIterator();
    for( unsigned int index = 0; it.HasMoreElements(); ++index )
    {
        const kernel::AgentComposition& component = it.NextElement();
        AddSubordinates( index, component.GetCount(), component.GetType() );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::AddSubordinates
// Created: SBO 2011-03-22
// -----------------------------------------------------------------------------
void Agent::AddSubordinates( unsigned int index, unsigned int count, const kernel::ComponentType& type )
{
    DtSilentEntityList* list = new DtSilentEntityList();
    list->setEntityType( publisher_->asr()->entityType() ); // $$$$ SBO 2011-03-22: resolve component type
    list->setNumberOfEntities( count );
    publisher_->asr()->setSilentEntityList( index, *list );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Agent::Update( const sword::UnitAttributes& message )
{
    if( IsTrueAggregate() )
    {
        UpdateLocation( message );
        publisher_->tick();
    }
    if( message.has_position() )
        SetAggregated( disaggregation_.IsAggregatedLocation( message.position().latitude(), message.position().longitude() ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::UpdateLocation
// Created: SBO 2011-03-22
// -----------------------------------------------------------------------------
void Agent::UpdateLocation( const sword::UnitAttributes& message )
{
    DtGeodeticCoord coord;
    const DtVector& vector = publisher_->asr()->location();
    if( vector != DtVector::zero() )
        coord.setGeocentric( vector );
    if( message.has_altitude() )
        coord.setAlt( message.altitude() );
    if( message.has_position() )
    {
        coord.setLat( DtDeg2Rad( message.position().latitude() ) );
        coord.setLon( DtDeg2Rad( message.position().longitude() ) );
    }
    if( message.has_direction() )
    {
        heading_ = message.direction().heading();
        DtDcm topoToGeoc;
        DtLatLon_to_TopoToGeoc( coord, topoToGeoc );
        DtTaitBryan geocOrient;
        DtEulerToEuler( DtTaitBryan( DtDeg2Rad( heading_ ), 0, 0 ), topoToGeoc, &geocOrient );
        publisher_->asr()->setOrientation( geocOrient );
    }
    publisher_->asr()->setLocation( coord.geocentric() );
}

// -----------------------------------------------------------------------------
// Name: Agent::SetAggregated
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Agent::SetAggregated( bool aggregated )
{
    if( ! IsTrueAggregate() && aggregated )
    {
        if( reflected_ )
            if( DtAggregateStateRepository* asr = reflected_->aggregateStateRep() )
            {
                DestroyPseudoAggregate();
                vrForces_.DestroyPseudoAggregate( asr->entityId() );
            }
    }
    else if( IsTrueAggregate() && ! aggregated )
        vrForces_.CreatePseudoAggregate( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::IsTrueAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
bool Agent::IsTrueAggregate() const
{
    return publisher_.get() != 0;
}

// -----------------------------------------------------------------------------
// Name: Agent::CreatePseudoAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Agent::CreatePseudoAggregate( DtVrfRemoteController& controller, const DtSimulationAddress& address )
{
    DtEntityType type = publisher_->asr()->entityType();
    type.setKind( type.kind() + 10 );
    vrForces_.RegisterReflectedCreationListener( *this );
    controller.createAggregate( &Agent::OnCreatePseudoAggregate, this
                              , type
                              , publisher_->asr()->location()
                              , publisher_->asr()->forceId()
                              , heading_
                              , publisher_->asr()->entityId().string()
                              , publisher_->asr()->markingText()
                              , address );
    for( int type = 0; type < publisher_->asr()->numberOfSilentEntities(); ++type )
    {
        DtSilentEntityList list;
        if( publisher_->asr()->getSilentEntityList( type, list ) )
            for( int entity = 0; entity < list.numberOfEntities(); ++entity )
            {
                const std::string id = boost::lexical_cast< std::string >( type * 1000 + entity );
                AddSubordinateEntity( controller, address, list.entityType(), id );
            }
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::AddSubordinateEntity
// Created: SBO 2011-03-22
// -----------------------------------------------------------------------------
void Agent::AddSubordinateEntity( DtVrfRemoteController& controller, const DtSimulationAddress& address, const DtEntityType& type, const std::string& identifier )
{
    subordinates_.push_back( boost::shared_ptr< Subordinate >( new Subordinate( type, *publisher_, heading_, identifier, controller, address ) ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DestroyPseudoAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Agent::DestroyPseudoAggregate()
{
    if( reflected_ )
    {
        subordinates_.clear();
        DtEntityType type( reflected_->asr()->entityType() );
        type.setKind( type.kind() - 10 );
        publisher_.reset( new DtAggregatePublisher( type, &connection_, reflected_->asr()->algorithm(), reflected_->asr()->forceId() ) );
        publisher_->asr()->setMarkingText( reflected_->asr()->markingText() );
        publisher_->asr()->setLocation( reflected_->asr()->location() );
        publisher_->asr()->setOrientation( reflected_->asr()->orientation() );
        CreateSubordinates( type_ );
        publisher_->tick();
        reflectedId_ = DtEntityIdentifier::nullId();
        reflected_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::OnCreateReflected
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
bool Agent::OnCreateReflected( DtReflectedAggregate* obj )
{
    if( reflected_ )
        return true;
    if( reflectedId_ != DtEntityIdentifier::nullId() )
        if( obj->aggregateStateRep()->entityId() == reflectedId_ )
        {
            reflected_ = obj;
            return true;
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Agent::OnCreatePseudoAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Agent::OnCreatePseudoAggregate( const DtString& /*name*/, const DtEntityIdentifier& id, void* usr )
{
    if( Agent* that = static_cast< Agent* >( usr ) )
    {
        that->publisher_.reset();
        that->reflectedId_ = id;
        if( !that->reflected_ )
            that->reflected_ = that->vrForces_.Find( id );
    }
}
