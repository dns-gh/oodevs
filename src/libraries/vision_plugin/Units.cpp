// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Units.h"
#include "protocol/ClientSenders.h"
#include "protocol/ClientPublisher_ABC.h"

using namespace plugins::vision;

void Units::Register( dispatcher::ClientPublisher_ABC& publisher, const sword::UnitId& id )
{
    units_.insert( Value( &publisher, id.id() ) );
}

void Units::Unregister( dispatcher::ClientPublisher_ABC& publisher, const sword::UnitId& id )
{
    auto it = units_.find( boost::make_tuple( &publisher, id.id() ) );
    if( it != units_.end() )
        units_.erase( it );
}

void Units::Unregister( dispatcher::ClientPublisher_ABC& publisher )
{
    auto range = units_.equal_range( &publisher );
    units_.erase( range.first, range.second );
}

void Units::List( dispatcher::ClientPublisher_ABC& publisher, unsigned int start, std::size_t max, client::ListEnabledVisionConesAck& ack )
{
    auto begin = units_.lower_bound( boost::make_tuple( &publisher, start ) );
    auto end = units_.upper_bound( &publisher );
    for( auto it = begin; it != end && max > 0; ++it, --max )
        ack().add_units()->set_id( it->id_ );
    ack().set_count( static_cast< google::protobuf::uint32 >( units_.count( &publisher ) ) );
    ack().mutable_start()->set_id( start );
}

void Units::Send( unsigned int id, const sword::SimToClient& message )
{
    auto range = units_.get< 1 >().equal_range( id );
    for( auto it = range.first; it != range.second; ++it )
        it->publisher_->Send( message );
}
