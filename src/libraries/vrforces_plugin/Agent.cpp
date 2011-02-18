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
#include "Facade.h"
#include "protocol/Protocol.h"
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
Agent::Agent( DtExerciseConn& connection, Facade& vrForces, const sword::UnitCreation& message )
    : connection_( connection )
    , vrForces_  ( vrForces )
    , id_        ( message.unit().id() )
    , heading_   ( 0 )
    , reflected_ ( 0 )
{
    DtEntityType type( DtPlatform, DtPlatformDomainLand, DtFrance, 3 /* platoon */, 2 /* armor */, message.pc(), 0 );
    publisher_.reset( new DtAggregatePublisher( type, &connection_, DtDrStatic, DtForceFriendly, message.name().c_str() ) );
    publisher_->asr()->setMarkingText( message.name().c_str() );
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

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Agent::Update( const sword::UnitAttributes& message )
{
    if( IsTrueAggregate() )
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
        publisher_->tick();
    }
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
            vrForces_.DestroyPseudoAggregate( reflected_->aggregateStateRep()->entityId() );
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
                              , heading_, 0, address );
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

// -----------------------------------------------------------------------------
// Name: Agent::OnDestroyPseudoAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Agent::OnDestroyPseudoAggregate( const DtString& /*name*/, const DtEntityIdentifier& id, void* usr )
{
    if( Agent* that = static_cast< Agent* >( usr ) )
    {
        if( that->reflected_ )
        {
            DtEntityType type( that->reflected_->asr()->entityType() );
            type.setKind( type.kind() - 10 );
            that->publisher_.reset( new DtAggregatePublisher( type, &that->connection_, that->reflected_->asr()->algorithm(), that->reflected_->asr()->forceId() ) );
            that->publisher_->asr()->setMarkingText( that->reflected_->asr()->markingText() );
            that->publisher_->asr()->setLocation( that->reflected_->asr()->location() );
            that->publisher_->asr()->setOrientation( that->reflected_->asr()->orientation() );
            that->publisher_->tick();
            
        }
        that->reflectedId_ = DtEntityIdentifier::nullId();
        that->reflected_ = 0;
    }
}
