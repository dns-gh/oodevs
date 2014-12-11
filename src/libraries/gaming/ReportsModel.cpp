// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ReportsModel.h"
#include "AgentsModel.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Time_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/MessageParameters.h"
#include "protocol/Protocol.h"
#include <boost/optional.hpp>

ReportsModel::ReportsModel( Publisher_ABC& publisher, AgentsModel& agents,
                            const kernel::Profile_ABC& profile, const kernel::Time_ABC& time )
    : publisher_ ( publisher )
    , agents_    ( agents )
    , profile_   ( profile )
    , time_      ( time )
    , tick_      ( 0 )
    , context_   ( 0 )
{
    publisher_.Register( Publisher_ABC::T_ReplayHandler( [&]( const sword::ReplayToClient& message )
    {
        if( message.message().has_control_skip_to_tick_ack() )
        {
            tick_ = message.message().control_skip_to_tick_ack().tick();
            Purge();
            SendRequest( ++context_ );
        }
        else if( message.message().has_list_reports_ack() )
        {
            const auto context = message.context();
            if( context != context_ )
                return;
            FillReports( message.message().list_reports_ack() );
        }
    } ) );
    publisher.Register( Publisher_ABC::T_SimHandler( [&]( const sword::SimToClient& message, bool /*ack*/ )
    {
        if( message.message().has_report() )
            AddMessage( message.message().report() );
        else if( message.message().has_trace() )
            AddMessage( message.message().trace() );
    } ) );
}

ReportsModel::~ReportsModel()
{
    // NOTHING
}

namespace
{
    const unsigned int maxCount = 500u;
}

void ReportsModel::SendRequest( int context, unsigned int report )
{
    sword::ClientToReplay message;
    message.set_context( context );
    auto& list = *message.mutable_message()->mutable_list_reports();
    list.set_max_count( maxCount );
    list.set_tick( tick_ );
    if( report != 0 )
        list.set_report( report );
    publisher_.Send( message );
}

void ReportsModel::FillReports( const sword::ListReportsAck& ack )
{
    for( int i = 0; i < ack.reports_size(); ++i )
    {
        const auto& report = ack.reports( i );
        const auto id = protocol::TryGetTasker( report.source() );
        if( !id )
            continue;
        auto& container = messages_[ *id ];
        const Message msg( *id, report, time_.GetDateTime() );
        container.push_front( msg );
        AddUnreadReports( *id );
    }
    if( ack.has_next_report() )
        SendRequest( context_, ack.next_report() );
}

const std::deque< ReportsModel::Message >& ReportsModel::GetReports( unsigned int entity ) const
{
    const static std::deque< ReportsModel::Message > emptyMessages;
    auto it = messages_.find( entity );
    if( it != messages_.end() )
        return it->second;
    return emptyMessages;
}

bool ReportsModel::HasUnreadReports( unsigned int entity ) const
{
    return entitiesSet_.find( entity ) != entitiesSet_.end();
}

size_t ReportsModel::UnreadReports() const
{
    return entities_.size();
}

void ReportsModel::Clear( unsigned int entity )
{
    ReadReports( entity );
    messages_.erase( entity );
}

void ReportsModel::ClearTraces( unsigned int entity )
{
    auto it = messages_.find( entity );
    if( it != messages_.end() )
    {
        auto& list = it->second;
        for( auto msg = list.begin(); msg != list.end(); )
        {
            if( msg->trace_.IsInitialized() )
                msg = list.erase( msg );
            else
                ++msg;
        }
    }
}

void ReportsModel::AddUnreadReports( unsigned int id )
{
    if( !HasUnreadReports( id ) )
        if( const auto* entity = agents_.FindAllAgent( id ) )
            if( profile_.IsVisible( *entity ) )
            {
                entities_.push_back( id );
                entitiesSet_.insert( id );
            }
}

void ReportsModel::ReadReports()
{
    entitiesSet_.erase( entities_.front() );
    entities_.pop_front();
}

void ReportsModel::ReadReports( unsigned int entity )
{
    auto it = entitiesSet_.find( entity );
    if( it == entitiesSet_.end() )
        return;
    entitiesSet_.erase( entity );

    auto itEntity = std::find( entities_.begin(), entities_.end(), entity );
    if( itEntity != entities_.end() )
        entities_.erase( itEntity );
}

unsigned int ReportsModel::NextUnreadReports() const
{
    return entities_.front();
}

void ReportsModel::UpdateUnreadReports()
{
    std::deque< unsigned int > filtered;
    std::unordered_set< unsigned int > filteredSet;

    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        if( const auto* entity = agents_.FindAllAgent( *it ) )
            if( profile_.IsVisible( *entity ) )
            {
                filtered.push_back( *it );
                filteredSet.insert( *it );
            }
    std::swap( filtered, entities_ );
    std::swap( filteredSet, entitiesSet_ );
}

void ReportsModel::Purge()
{
    messages_.clear();
    entities_.clear();
    entitiesSet_.clear();
}

template< typename T >
void ReportsModel::AddMessage( const T& message )
{
    const auto id = protocol::TryGetTasker( message.source() );
    if( !id )
        return;
    const Message msg( *id , message, time_.GetDateTime() );
    messages_[ msg.entity_ ].push_back( msg );
    AddUnreadReports( msg.entity_ );
}
