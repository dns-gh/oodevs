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
#include "protocol/Protocol.h"

namespace
{
    unsigned int GetId( const sword::Tasker& id )
    {
        if( id.has_unit() )
            return id.unit().id();
        else if( id.has_automat() )
            return id.automat().id();
        else if( id.has_crowd() )
            return id.crowd().id();
        return 0u;
    }
}

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
        {
            const auto& report = message.message().report();
            const Message msg( GetId( report.source() ), report, time_.GetDateTime() );
            AddMessage( msg );
        }
        else if( message.message().has_trace() )
        {
            const auto& trace = message.message().trace();
            const Message msg( GetId( trace.source() ), trace, time_.GetDateTime() );
            AddMessage( msg );
        }
    } ) );
}

ReportsModel::~ReportsModel()
{
    // NOTHING
}

namespace
{
    const unsigned int maxCount = 50u;
    const std::vector< ReportsModel::Message > emptyMessages;
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
        if( const auto id = GetId( report.source() ) )
        {
            auto& container = messages_[ id ];
            const Message msg( GetId( report.source() ), report, time_.GetDateTime() );
            container.insert( container.begin(), msg );
            AddUnreadReports( id );
        }
    }
    if( ack.has_next_report() )
        SendRequest( context_, ack.next_report() );
}

const ReportsModel::T_Messages& ReportsModel::GetReports( unsigned int entity ) const
{
    auto it = messages_.find( entity );
    if( it != messages_.end() )
        return it->second;
    return emptyMessages;
}

bool ReportsModel::HaveUnreadReports( unsigned int entity ) const
{
    return std::find( entities_.begin(), entities_.end(), entity ) != entities_.end();
}

size_t ReportsModel::UnreadReports() const
{
    return entities_.size();
}

void ReportsModel::Clear( unsigned int entity )
{
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
    if( !HaveUnreadReports( id ) )
        if( const auto* entity = agents_.FindAllAgent( id ) )
            if( profile_.IsVisible( *entity ) )
                entities_.push_back( id );
}

void ReportsModel::ReadReports()
{
    entities_.pop_front();
}

void ReportsModel::ReadReports( unsigned int entity )
{
    auto it = std::find( entities_.begin(), entities_.end(), entity );
    if( it != entities_.end() )
        entities_.erase( it );
}

unsigned int ReportsModel::NextUnreadReports() const
{
    return entities_.front();
}

void ReportsModel::UpdateUnreadReports()
{
    std::deque< unsigned int > filtered;
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        if( const auto* entity = agents_.FindAllAgent( *it ) )
            if( profile_.IsVisible( *entity ) )
                filtered.push_back( *it );
    std::swap( filtered, entities_ );
}

void ReportsModel::Purge()
{
    messages_.clear();
    entities_.clear();
}

void ReportsModel::AddMessage( const Message& message )
{
    if( !message.entity_ )
        return;
    messages_[ message.entity_ ].push_back( message );
    AddUnreadReports( message.entity_ );
}
