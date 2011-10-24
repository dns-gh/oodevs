// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Model.h"
#include "Agent.h"
#include "Automat.h"
#include "BoundaryLimit.h"
#include "DetectionEvents.h"
#include "Drawing.h"
#include "FireEngagement.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Obstacle.h"
#include "Party.h"
#include "PhaseLine.h"
#include "Report.h"
#include "Simulation.h"
#include "Task.h"
#include "Weather.h"
#include "WiseHelpers.h"
#include "protocol/Messenger.h"
#include "protocol/Simulation.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Model::Model( CWISEDriver& driver, const WISE_HANDLE& database, SwordMessagePublisher_ABC& publisher )
    : driver_     ( driver )
    , database_   ( database )
    , publisher_  ( publisher )
    , simulation_ ( new Simulation( publisher ) )
    , weather_( new Weather() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Model::~Model()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMessage
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Model::OnReceiveMessage( const sword::SimToClient& message )
{
    const sword::SimToClient::Content& content = message.message();
    // simulation
    if( content.has_control_begin_tick() )
        simulation_->Update( driver_, database_, GetTime(), content.control_begin_tick() );
    else if( content.has_control_resume_ack() )
        simulation_->Update( driver_, database_, GetTime(), content.control_resume_ack() );
    else if( content.has_control_pause_ack() )
        simulation_->Update( driver_, database_, GetTime(), content.control_pause_ack() );
    else if( content.has_control_information() )
        simulation_->Create( driver_, database_, GetTime(), content.control_information() );
    else if( content.has_control_change_time_factor_ack() )
        simulation_->Update( driver_, database_, GetTime(), content.control_change_time_factor_ack() );
    // parties
    else if( content.has_party_creation() )
        Create( parties_, content.party_creation() );
    else if( content.has_change_diplomacy_ack() )
        Update( parties_, content.change_diplomacy_ack().party1().id(), content.change_diplomacy_ack() );
    else if( content.has_change_diplomacy() )
        Update( parties_, content.change_diplomacy().party1().id(), content.change_diplomacy() );
    // knowledge groups
    else if( content.has_knowledge_group_creation() )
        Create( knowledgeGroups_, content.knowledge_group_creation() );
    else if( content.has_knowledge_group_destruction() )
        Destroy( knowledgeGroups_, content.knowledge_group_destruction().knowledge_group().id() );
    else if( content.has_knowledge_group_update() )
        Update( knowledgeGroups_, content.knowledge_group_update().knowledge_group().id(), content.knowledge_group_update() );
    // formations
    else if( content.has_formation_creation() )
        Create( formations_, content.formation_creation() );
    else if( content.has_formation_destruction() )
        Destroy( formations_, content.formation_destruction().formation().id() );
    // automats
    else if( content.has_automat_creation() )
        Create( automats_, content.automat_creation() );
    else if( content.has_automat_destruction() )
        Destroy( automats_, content.automat_destruction().automat().id() );
    else if( content.has_automat_attributes() )
        Update( automats_, content.automat_attributes().automat().id(), content.automat_attributes() );
    else if( content.has_automat_change_superior() )
        Update( automats_, content.automat_change_superior().automat().id(), content.automat_change_superior() );
    else if( content.has_automat_change_knowledge_group() )
        Update( automats_, content.automat_change_knowledge_group().automat().id(), content.automat_change_knowledge_group() );
    // units
    else if( content.has_unit_creation() )
        Create( agents_, content.unit_creation() );
    else if( content.has_unit_destruction() )
        Destroy( agents_, content.unit_destruction().unit().id() );
    else if( content.has_unit_attributes() )
        Update( agents_, content.unit_attributes().unit().id(), content.unit_attributes() );
    else if( content.has_unit_change_superior() )
        Update( agents_, content.unit_change_superior().unit().id(), content.unit_change_superior() );
    // obstacles
    else if( content.has_object_creation() )
        Create( obstacles_, content.object_creation() );
    else if( content.has_object_destruction() )
        Destroy( obstacles_, content.object_destruction().object().id() );
    // reports
    else if( content.has_report() )
        Report::Trigger( driver_, database_, GetTime(), *this, content.report() );
    // fire engagements
    else if( content.has_start_unit_fire() )
        Model::CreateEvent( fireEngagements_, content.start_unit_fire() );
    else if( content.has_stop_unit_fire() )
        Update( fireEngagements_, content.stop_unit_fire().fire().id(), content.stop_unit_fire() );
    // detections
    else if( content.has_unit_detection() )
        DetectionEvents::Trigger( driver_, database_, GetTime(), *this, content.unit_detection() );
    else if( content.has_object_detection() )
        DetectionEvents::Trigger( driver_, database_, GetTime(), *this, content.object_detection() );
    // orders
    else if( content.has_unit_order() )
        Task::Notify( driver_, database_, GetTime(), *this, content.unit_order() );
    else if( content.has_automat_order() )
        Task::Notify( driver_, database_, GetTime(), *this, content.automat_order() );
    // weather
    else if( content.has_control_global_weather() )
        weather_->Update( driver_, database_, GetTime(), content.control_global_weather() );
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMessage
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Model::OnReceiveMessage( const sword::MessengerToClient& message )
{
    const sword::MessengerToClient::Content& content = message.message();
    if( content.has_limit_creation() )
        Create( boundaryLimits_, content.limit_creation() );
    else if( content.has_limit_destruction() )
        Destroy( boundaryLimits_, content.limit_destruction().id().id() );
    else if( content.has_phase_line_creation() )
        Create( phaseLines_, content.phase_line_creation() );
    else if( content.has_phase_line_destruction() )
        Destroy( phaseLines_, content.phase_line_destruction().id().id() );
    else if( content.has_shape_creation() )
        Create( drawings_, content.shape_creation() );
    else if( content.has_shape_destruction() )
        Destroy( drawings_, content.shape_destruction().id().id() );
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMessage
// Created: LGY 2011-05-20
// -----------------------------------------------------------------------------
void Model::OnReceiveMessage( const sword::AuthenticationToClient& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMessage
// Created: JSR 2011-10-20
// -----------------------------------------------------------------------------
void Model::OnReceiveMessage( const sword::DispatcherToClient& /*message*/ )
{
    // NOTHING
}

namespace
{
    template< class Entity >
    const Entity* Resolve( const std::map< unsigned long, Entity* >& entities, const unsigned long& id )
    {
        std::map< unsigned long, Entity* >::const_iterator it = entities.find( id );
        if( it != entities.end() )
            return it->second;
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveParty
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
const Party* Model::ResolveParty( const unsigned long& id ) const
{
    return Resolve( parties_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveKnowledgeGroup
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
const KnowledgeGroup* Model::ResolveKnowledgeGroup( const unsigned long& id ) const
{
    return Resolve( knowledgeGroups_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveFormation
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
const Formation* Model::ResolveFormation( const unsigned long& id ) const
{
    return Resolve( formations_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveAutomat
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
const Automat* Model::ResolveAutomat( const unsigned long& id ) const
{
    return Resolve( automats_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveAgent
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
const Agent* Model::ResolveAgent( const unsigned long& id ) const
{
    return Resolve( agents_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveObstacle
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
const Obstacle* Model::ResolveObstacle( const unsigned long& id ) const
{
    return Resolve( obstacles_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveEntity
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
const WiseEntity* Model::ResolveEntity( const WISE_HANDLE& handle ) const
{
    std::map< WISE_HANDLE, WiseEntity* >::const_iterator it = entities_.find( handle );
    if( it != entities_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Model::Reset
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Model::Reset()
{
    weather_->Destroy( driver_, database_ );
    simulation_->Destroy( driver_, database_ );
    Clear( phaseLines_ );
    Clear( boundaryLimits_ );
    Clear( drawings_ );
    Clear( obstacles_ );
    Clear( agents_ );
    Clear( automats_ );
    Clear( formations_ );
    Clear( knowledgeGroups_ );
    Clear( parties_ );
    entities_.clear();
}

// -----------------------------------------------------------------------------
// Name: Model::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
template< class Entity, class Message >
void Model::Create( std::map< WISE_HANDLE, Entity* >& entities, const Message& message )
{
    if( Entity* entity = new Entity( *this, message ) )
    {
        entity->Create( driver_, database_, GetTime() );
        entities[ entity->GetId() ] = entity;
        entities_[ entity->GetHandle() ] = entity;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::CreateEvent
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
template< class Event, class Message >
void Model::CreateEvent( std::map< unsigned long, Event* >& events, const Message& message )
{
    if( Event* ev = new Event( *this, message ) )
        events[ ev->GetId() ] = ev;
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
template< class Entity, class Message >
void Model::Update( std::map< unsigned long, Entity* >& entities, unsigned long id, const Message& message )
{
    std::map< unsigned long, Entity* >::iterator it = entities.find( id );
    if( it != entities.end() )
        it->second->Update( driver_, database_, GetTime(), message );
}

// -----------------------------------------------------------------------------
// Name: Model::Clear
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
template< class Entity >
void Model::Clear( std::map< unsigned long, Entity* >& entities )
{
    for( std::map< unsigned long, Entity* >::iterator it = entities.begin(); it != entities.end(); ++it )
    {
        it->second->Destroy( driver_, database_ );
        delete it->second;
    }
    entities.clear();
}

// -----------------------------------------------------------------------------
// Name: Model::Destroy
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
template< class Entity >
void Model::Destroy( std::map< unsigned long, Entity* >& entities, unsigned long id )
{
    std::map< unsigned long, Entity* >::iterator it = entities.find( id );
    if( it != entities.end() )
    {
        it->second->Destroy( driver_, database_ );
        delete it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::GetTime
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
timeb Model::GetTime()
{
    timeb result;
    memset( &result, 0, sizeof( timeb ) );
    ftime( &result );
    try
    {
        CHECK_WISE_RESULT_EX( driver_.GetSink()->GetDatabaseTime( database_, result ) );
    }
    catch( WISE_RESULT& error )
    {
        driver_.NotifyErrorMessage( L"Failed to retrieve database time.", error );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveEvent
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Model::OnReceiveEvent( const WISE_HANDLE& handle )
{
    std::wstring type;
    wise::FetchEventAttribute( driver_, database_, handle, L"WISE_TEMPLATE_EVENT_TYPE", type );
    if( type == L"TaskCreationRequest" )
        Task::Issue( driver_, database_, *this, publisher_, handle );
    else
        driver_.NotifyWarningMessage( L"Unable to process event of type '" + type + L"'.", MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_INVALID_FORMAT ) );
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveUpdate
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void Model::OnReceiveUpdate( const WISE_HANDLE& object, const WISE_HANDLE& attribute, const CWISEValueUnion& value )
{
    std::map< WISE_HANDLE, WiseEntity* >::iterator it = entities_.find( object );
    if( it != entities_.end() )
        it->second->Update( publisher_, attribute, value );
    else if( !simulation_->Update( attribute, value ) )
        driver_.NotifyWarningMessage( L"Unable to process attribute update.", MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_INVALID_FORMAT ) );
}
