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
#include "FireEngagement.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Obstacle.h"
#include "Party.h"
#include "PhaseLine.h"
#include "Simulation.h"
#include "TaskFactory.h"
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
    : driver_( driver )
    , database_( database )
    , simulation_( new Simulation( publisher ) )
    , taskFactory_( new TaskFactory( *this, publisher ) )
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
void Model::OnReceiveMessage( const MsgsSimToClient::MsgSimToClient& message )
{
    if( message.message().has_control_begin_tick() )
        simulation_->Update( driver_, database_, GetTime(), message.message().control_begin_tick() );
    else if( message.message().has_party_creation() )
        Create( parties_, message.message().party_creation() );
    else if( message.message().has_party_creation() )
        Create( knowledgeGroups_, message.message().knowledge_group_creation() );
    else if( message.message().has_formation_creation() )
        Create( formations_, message.message().formation_creation() );
    else if( message.message().has_automat_creation() )
        Create( automats_, message.message().automat_creation() );
    else if( message.message().has_unit_creation() )
        Create( agents_, message.message().unit_creation() );
    else if( message.message().has_unit_attributes() )
        Update( agents_, message.message().unit_attributes().unit().id(), message.message().unit_attributes() );
    else if( message.message().has_control_information() )
        simulation_->Create( driver_, database_, GetTime(), message.message().control_information() );
    else if( message.message().has_control_change_time_factor_ack() )
        simulation_->Update( driver_, database_, GetTime(), message.message().control_change_time_factor_ack() );
    else if( message.message().has_start_unit_fire() )
        Model::CreateEvent( fireEngagements_, message.message().start_unit_fire() );
    else if( message.message().has_stop_unit_fire() )
        Update( fireEngagements_, message.message().stop_unit_fire().fire().id(), message.message().stop_unit_fire() );
    else if( message.message().has_unit_detection() )
        DetectionEvents::Trigger( driver_, database_, GetTime(), *this, message.message().unit_detection() );
    else if( message.message().has_object_detection() )
        DetectionEvents::Trigger( driver_, database_, GetTime(), *this, message.message().object_detection() );
    else if( message.message().has_object_creation() )
        Create( obstacles_, message.message().object_creation() );
    else if( message.message().has_object_destruction() )
        Destroy( obstacles_, message.message().object_destruction().object().id() );
    else if( message.message().has_change_diplomacy() )
        Update( parties_, message.message().change_diplomacy().party1().id(), message.message().change_diplomacy() );
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMessage
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Model::OnReceiveMessage( const MsgsMessengerToClient::MsgMessengerToClient& message )
{
    if( message.message().has_limit_creation() )
        Create( boundaryLimits_, message.message().limit_creation() );
    else if( message.message().has_limit_destruction() )
        Destroy( boundaryLimits_, message.message().limit_destruction().id().id() );
    else if( message.message().has_lima_creation() )
        Create( phaseLines_, message.message().lima_creation() );
    else if( message.message().has_lima_destruction() )
        Destroy( phaseLines_, message.message().lima_destruction().id().id() );
}

namespace
{
    template< class Entity >
    const Entity* ResolveEntity( const std::map< unsigned long, Entity* >& entities, const unsigned long& id )
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
    return ResolveEntity( parties_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveKnowledgeGroup
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
const KnowledgeGroup* Model::ResolveKnowledgeGroup( const unsigned long& id ) const
{
    return ResolveEntity( knowledgeGroups_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveFormation
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
const Formation* Model::ResolveFormation( const unsigned long& id ) const
{
    return ResolveEntity( formations_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveAutomat
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
const Automat* Model::ResolveAutomat( const unsigned long& id ) const
{
    return ResolveEntity( automats_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveAgent
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
const Agent* Model::ResolveAgent( const unsigned long& id ) const
{
    return ResolveEntity( agents_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::ResolveObstacle
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
const Obstacle* Model::ResolveObstacle( const unsigned long& id ) const
{
    return ResolveEntity( obstacles_, id );
}

// -----------------------------------------------------------------------------
// Name: Model::Reset
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Model::Reset()
{
    simulation_->Destroy( driver_, database_ );
    Clear( phaseLines_ );
    Clear( boundaryLimits_ );
    Clear( obstacles_ );
    Clear( agents_ );
    Clear( automats_ );
    Clear( formations_ );
    Clear( knowledgeGroups_ );
    Clear( parties_ );
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
    WISE_HANDLE attribute = WISE_INVALID_HANDLE;
    CHECK_WISE_RESULT_EX( driver_.GetSink()->GetEventAttributeHandle( database_, handle, L"WISE_TEMPLATE_EVENT_TYPE", attribute ) );
    CHECK_WISE_RESULT_EX( driver_.GetSink()->GetEventAttributeValue( database_, handle, attribute, type ) );
    if( type == L"SimulationStateUpdate" )
        simulation_->OnUpdateState( driver_, database_, handle );
    else if( type == L"SimulationAccelerationFactorUpdate" )
        simulation_->OnUpdateAccelerationFactor( driver_, database_, handle );
    else if( type == L"TaskCreation" )
        taskFactory_->OnCreate( driver_, database_, handle );
    else
        driver_.NotifyWarningMessage( L"Unable to process event of type '" + type + L"'.", MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_INVALID_FORMAT ) );
}
