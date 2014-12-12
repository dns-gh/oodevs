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
{
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

const std::deque< ReportsModel::Message >& ReportsModel::GetTraces( unsigned int entity ) const
{
    const static std::deque< ReportsModel::Message > emptyMessages;
    auto it = traces_.find( entity );
    if( it != traces_.end() )
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

void ReportsModel::ClearTraces( unsigned int entity )
{
    traces_.erase( entity );
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
    traces_.clear();
    entities_.clear();
    entitiesSet_.clear();
}

template< typename T >
void ReportsModel::AddMessage( const T& message )
{
    const auto id = protocol::TryGetTasker( message.source() );
    if( !id )
        return;
    AddUnreadReports( *id );
    const Message msg( *id , message, time_.GetDateTime() );
    if( msg.trace_.IsInitialized() )
        traces_[ *id ].push_back( msg );
}
