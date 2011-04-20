// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ResourceNetworkAttribute.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( const sword::UrbanAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( const sword::ObjectAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute destructor
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::~ResourceNetworkAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Update
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Update( const sword::UrbanAttributes& message )
{
    if( !message.has_infrastructures() )
        return;
    for( int i = 0; i < message.infrastructures().resource_network_size(); ++i )
        Update( message.infrastructures().resource_network( i ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Send
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Send( sword::UrbanAttributes& message ) const
{
    for( std::map< std::string, ResourceNetwork >::const_iterator it = resourceMap_.begin(); it != resourceMap_.end(); ++it )
        Send( *message.mutable_infrastructures()->add_resource_network(), it->second );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Update
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Update( const sword::ObjectAttributes& message )
{
    if( !message.has_resource_networks() )
        return;
    for( int i = 0; i < message.resource_networks().network_size(); ++i )
        Update( message.resource_networks().network( i ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Send
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Send( sword::ObjectAttributes& message ) const
{
    message.mutable_resource_networks();
    for( std::map< std::string, ResourceNetwork >::const_iterator it = resourceMap_.begin(); it != resourceMap_.end(); ++it )
        Send( *message.mutable_resource_networks()->add_network(), it->second );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Update
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Update( const sword::ResourceNetwork& from )
{
    ResourceNetwork& to = resourceMap_[ from.resource().name() ];
    to.resource_ = from.resource().name();
    to.enabled_ = from.enabled();
    to.maxStock_ = from.has_max_stock() ? from.max_stock() : 0;
    to.stock_ = from.has_stock() ? from.stock() : 0;
    to.production_ = from.has_production() ? from.production() : 0;
    to.consumption_ = from.has_consumption() ? from.consumption() : 0;
    to.critical_ = from.has_critical() ? from.critical() : false;
    to.maxConsumption_ = from.has_max_consumption() ? from.max_consumption() : 0;
    to.currentConsumption_ = from.has_current_consumption() ? from.current_consumption() : 0;
    for( int j = 0; j < from.link_size(); ++j )
    {
        const sword::ResourceNetwork_Link& linkFrom = from.link( j );
        std::vector< ResourceNetwork::Link >::iterator it = to.links_.begin();
        for( ; it != to.links_.end(); ++it )
            if( it->objectId_ == linkFrom.object().id() )
                break;
        if( it == to.links_.end() )
            it = to.links_.insert( it, ResourceNetwork::Link() );
        it->objectId_ = linkFrom.object().id();
        it->capacity_ = linkFrom.capacity();
        it->flow_ = linkFrom.flow();
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Send
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Send( sword::ResourceNetwork& message, const ResourceNetwork& network ) const
{
    message.mutable_resource()->set_name( network.resource_ );
    message.set_enabled( network.enabled_ );
    if( network.maxStock_ )
        message.set_max_stock( network.maxStock_ );
    if( network.stock_ )
        message.set_stock( network.stock_ );
    if( network.production_ )
        message.set_production( network.production_ );
    if( network.currentConsumption_ )
        message.set_current_consumption( network.currentConsumption_ );
    if( network.maxConsumption_ )
        message.set_max_consumption( network.maxConsumption_ );
    if( network.consumption_ )
    {
        message.set_consumption( network.consumption_ );
        message.set_critical( network.critical_ );
    }
    for( std::vector< ResourceNetwork::Link >::const_iterator it = network.links_.begin(); it != network.links_.end(); ++it )
    {
        sword::ResourceNetwork_Link* link = message.add_link();
        link->mutable_object()->set_id( it->objectId_ );
        link->set_capacity( it->capacity_ );
        link->set_flow( it->flow_ );
    }
}
