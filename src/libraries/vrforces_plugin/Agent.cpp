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
#include "AggregatedPosition.h"
#include "AggregatedState.h"
#include "DisaggregationStrategy_ABC.h"
#include "Facade.h"
#include "ForceResolver_ABC.h"
#include "Subordinate.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/SimulationSenders.h"
#include "rpr/EntityType.h"
#include "rpr/EntityTypeResolver_ABC.h"
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

#pragma warning( disable: 4355 )

using namespace plugins::vrforces;

namespace
{
    struct Serializer
    {
        explicit Serializer( const std::string& separator ) : separator_( separator ) {}

        template< typename T >
        Serializer& operator<<( const T& value )
        {
            if( !result_.empty() )
                result_ += separator_;
            result_ += boost::lexical_cast< std::string >( static_cast< unsigned int >( value ) );
            return *this;
        }
        std::string separator_;
        std::string result_;
    };

    std::string ToString( const rpr::EntityType& type )
    {
        Serializer serializer( ":" );
        type.Serialize( serializer );
        return serializer.result_;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
Agent::Agent( const kernel::Agent_ABC& agent, DtExerciseConn& connection, Facade& vrForces, const sword::UnitCreation& message, const ForceResolver_ABC& forces, const DisaggregationStrategy_ABC& disaggregation, const rpr::EntityTypeResolver_ABC& entityTypes, dispatcher::SimulationPublisher_ABC& simulation )
    : disaggregation_( disaggregation )
    , swordPublisher_( simulation )
    , connection_    ( connection )
    , vrForces_      ( vrForces )
    , id_            ( message.unit().id() )
    , heading_       ( 0 )
    , reflectedId_   ( DtEntityIdentifier::nullId() )
    , reflected_     ( 0 )
    , type_          ( agent.GetType() )
    , entityTypes_   ( entityTypes )
    , position_      ( new AggregatedPosition( *this ) )
    , state_         ( new AggregatedState( *this ) )
{
    std::stringstream name;
    name << message.automat().id() << ":"<< id_ << "/" << message.name().c_str();
    DtEntityType type( ToString( entityTypes_.Find( agent.GetType().GetName() ) ).c_str() );
    aggregatePublisher_.reset( new DtAggregatePublisher( type, &connection_, DtDrStatic, forces.Resolve( id_ ), name.str().c_str() ) );
    aggregatePublisher_->asr()->setMarkingText( name.str().c_str() );
    CreateSubordinates( type_ );
    aggregatePublisher_->tick();
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
    aggregatePublisher_->asr()->setNumberOfSilentEntities( CountSubordinates( type ) );
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
    list->setEntityType( ToString( entityTypes_.Find( type.GetName() ) ).c_str() );
    list->setNumberOfEntities( count );
    aggregatePublisher_->asr()->setSilentEntityList( index, *list );
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
        aggregatePublisher_->tick();
    }
    if( message.has_position() )
        SetAggregated( disaggregation_.IsAggregatedLocation( message.position().latitude(), message.position().longitude() ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: SBO 2011-04-07
// -----------------------------------------------------------------------------
void Agent::Update( const sword::UnitPathFind& message )
{
    if( message.has_path() )
    {
        const sword::CoordLatLongList& points = message.path().location().coordinates();
        if( points.elem_size() > 0 )
        {
            const sword::CoordLatLong& last = points.elem( points.elem_size() - 1 );
            DtGeodeticCoord coord;
            coord.setLat( DtDeg2Rad( last.latitude() ) );
            coord.setLon( DtDeg2Rad( last.longitude() ) );
            destination_ = coord.geocentric();
        }
    }
    else
        destination_.setToZero();
}

// -----------------------------------------------------------------------------
// Name: Agent::UpdateLocation
// Created: SBO 2011-03-22
// -----------------------------------------------------------------------------
void Agent::UpdateLocation( const sword::UnitAttributes& message )
{
    DtGeodeticCoord coord;
    const DtVector& vector = aggregatePublisher_->asr()->location();
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
        aggregatePublisher_->asr()->setOrientation( geocOrient );
    }
    aggregatePublisher_->asr()->setLocation( coord.geocentric() );
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
    {
        CancelMission();
        vrForces_.CreatePseudoAggregate( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::IsTrueAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
bool Agent::IsTrueAggregate() const
{
    return aggregatePublisher_.get() != 0;
}

// -----------------------------------------------------------------------------
// Name: Agent::CreatePseudoAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Agent::CreatePseudoAggregate( DtVrfRemoteController& controller, const DtSimulationAddress& address )
{
    DtEntityType type = aggregatePublisher_->asr()->entityType();
    if( type.kind() < 10 )
        type.setKind( type.kind() + 10 );
    vrForces_.AddListener( *this );
    controller.createAggregate( &Agent::OnCreatePseudoAggregate, this
                              , type
                              , aggregatePublisher_->asr()->location()
                              , aggregatePublisher_->asr()->forceId()
                              , heading_
                              , DtString::nullString()
                              , aggregatePublisher_->asr()->markingText()
                              , address );
    for( int type = 0; type < aggregatePublisher_->asr()->numberOfSilentEntities(); ++type )
    {
        DtSilentEntityList list;
        if( aggregatePublisher_->asr()->getSilentEntityList( type, list ) )
            for( int entity = 0; entity < list.numberOfEntities(); ++entity )
            {
                const std::string id = boost::lexical_cast< std::string >( type * 1000 + entity );
                AddSubordinateEntity( controller, address, list.entityType(), id );
            }
    }
    DtInfo << "Request pseudo aggregate with id: " << aggregatePublisher_->asr()->entityId() << "type: " << type.string() << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Agent::AddSubordinateEntity
// Created: SBO 2011-03-22
// -----------------------------------------------------------------------------
void Agent::AddSubordinateEntity( DtVrfRemoteController& controller, const DtSimulationAddress& address, const DtEntityType& type, const std::string& identifier )
{
    subordinates_.push_back( boost::shared_ptr< Subordinate >( new Subordinate( type, *aggregatePublisher_, heading_, identifier, controller, address, vrForces_, *this ) ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DestroyPseudoAggregate
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
void Agent::DestroyPseudoAggregate()
{
    if( reflected_ )
    {
        position_->Clear();
        subordinates_.clear();
        DtAggregateStateRepository* copy = new DtAggregateStateRepository( *reflected_->asr() );
        DtEntityType type( ToString( entityTypes_.Find( type_.GetName() ) ).c_str() );
        copy->setAggregateType( type );
        copy->setAlgorithm( DtDrStatic );
        aggregatePublisher_.reset( new DtAggregatePublisher( copy, &connection_ ) );
        CreateSubordinates( type_ );
        reflectedId_ = DtEntityIdentifier::nullId();
        reflected_ = 0;
        aggregatePublisher_->tick();
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::NotifyCreated
// Created: SBO 2011-01-21
// -----------------------------------------------------------------------------
bool Agent::NotifyCreated( DtReflectedAggregate& aggregate )
{
    if( reflected_ )
        return true;
    if( reflectedId_ != DtEntityIdentifier::nullId() )
        if( aggregate.asr()->entityId() == reflectedId_ )
        {
            reflected_ = &aggregate;
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
        that->aggregatePublisher_.reset();
        that->reflectedId_ = id;
        if( !that->reflected_ )
            that->vrForces_.Lookup( id, *that );
        for( T_Subordinates::iterator it = that->subordinates_.begin(); it != that->subordinates_.end(); ++it )
        {
            (*it)->SetSuperior( id );
            (*it)->SetDestination( that->destination_ );
        }
        DtInfo << "Pseudo aggregate created with identifier: " << id.string() << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::CancelMission
// Created: SBO 2011-03-25
// -----------------------------------------------------------------------------
void Agent::CancelMission()
{
    if( IsTrueAggregate() )
    {
        DtGeodeticCoord converter;
        converter.setGeocentric( aggregatePublisher_->asr()->location() );
        geometry::Point2d position( DtRad2Deg( converter.lat() ), DtRad2Deg( converter.lon() ) );
        MoveTo( position );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::MoveTo
// Created: SBO 2011-04-04
// -----------------------------------------------------------------------------
void Agent::MoveTo( const geometry::Point2d& position ) const
{
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_unit()->set_id( id_ );
    message().set_type( sword::UnitMagicAction_Type_move_to );
    sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
    parameter.set_null_value( false );
    sword::Location& location = *parameter.mutable_value()->Add()->mutable_point()->mutable_location();
    location.set_type( sword::Location::point );
    sword::CoordLatLong& coord = *location.mutable_coordinates()->add_elem();
    coord.set_latitude( position.X() );
    coord.set_longitude( position.Y() );
    message.Send( swordPublisher_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::NotifyUpdated
// Created: SBO 2011-04-04
// -----------------------------------------------------------------------------
void Agent::NotifyUpdated( const Subordinate& subordinate )
{
    if( !IsTrueAggregate() )
    {
        subordinate.Update( *position_ );
        subordinate.Update( *state_ );
    }
}

namespace
{
    sword::MissionParameter& MakeParameter( simulation::UnitMagicAction& message )
    {
        sword::MissionParameter& parameter = *message().mutable_parameters()->add_elem();
        parameter.set_null_value( true );
        return parameter;
    }

    void AddState( unsigned long id, unsigned int available, sword::MissionParameter::Value& parameter )
    {
        parameter.mutable_list()->Add()->set_identifier( id );
        parameter.mutable_list()->Add()->set_quantity( available );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::NotifyStateChanged
// Created: SBO 2011-04-11
// -----------------------------------------------------------------------------
void Agent::NotifyStateChanged() const
{
    simulation::UnitMagicAction message;
    message().mutable_tasker()->mutable_unit()->set_id( id_ );
    message().set_type( sword::UnitMagicAction_Type_partial_recovery );
    {
        // parameter 1: equipments
        std::map< unsigned long, unsigned int > available;
        tools::Iterator< const kernel::AgentComposition& > it = type_.CreateIterator();
        T_Subordinates::const_iterator itSub = subordinates_.begin();
        for( unsigned int index = 0; it.HasMoreElements(); ++index )
        {
            const kernel::AgentComposition& component = it.NextElement();
            unsigned int& undamaged = available[component.GetType().GetId()] = 0;
            for( unsigned int i = 0; i < component.GetCount() && itSub != subordinates_.end(); ++i, ++itSub )
                if( (*itSub)->IsUndamaged() )
                    ++undamaged;
        }
        sword::MissionParameter& parameter = MakeParameter( message );
        parameter.set_null_value( false );
        for( std::map< unsigned long, unsigned int >::const_iterator it = available.begin(); it != available.end(); ++it )
            AddState( it->first, it->second, *parameter.mutable_value()->Add() );
    }
    MakeParameter( message ); // parameter 2: personel
    MakeParameter( message ); // parameter 3: resources
    MakeParameter( message ); // parameter 4: ammo
    MakeParameter( message ); // parameter 5: stocks
    message.Send( swordPublisher_ );
    DtInfo << message().DebugString();
}
