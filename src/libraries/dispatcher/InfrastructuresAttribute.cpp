// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "InfrastructuresAttribute.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: InfrastructuresAttribute constructor
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
InfrastructuresAttribute::InfrastructuresAttribute( const MsgsSimToClient::MsgUrbanAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: InfrastructuresAttribute destructor
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
InfrastructuresAttribute::~InfrastructuresAttribute()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: InfrastructuresAttribute::Update
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void InfrastructuresAttribute::Update( const MsgsSimToClient::MsgUrbanAttributes& message )
{
    if( !message.has_infrastructures() )
        return;
    // Resource network
    for( int i = 0; i < message.infrastructures().resource_network_size(); ++i )
    {
        const MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork& from = message.infrastructures().resource_network( i );
        ResourceNetwork& to = resourceMap_[ from.type() ];
        to.type_ = from.type();
        to.enabled_ = from.enabled();
        to.maxStock_ = from.has_max_stock() ? from.max_stock() : 0;
        to.stock_ = from.has_stock() ? from.stock() : 0;
        to.production_ = from.has_production() ? from.production() : 0;
        to.consumption_ = from.has_consumption() ? from.consumption() : 0;
        to.critical_ = from.has_critical() ? from.critical() : false;
        for( int j = 0; j < from.link_size(); ++j )
        {
            const MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link& linkFrom = from.link( j );
            std::vector< ResourceNetwork::Link >::iterator it = to.links_.begin();
            for( ; it != to.links_.end(); ++it )
                if( it->kind_ == linkFrom.kind() && it->target_ == linkFrom.target_id() )
                    break;
            if( it == to.links_.end() )
                it = to.links_.insert( it, ResourceNetwork::Link() );
            it->kind_ = linkFrom.kind();
            it->target_ = linkFrom.target_id();
            it->capacity_ = linkFrom.capacity();
            it->flow_ = linkFrom.flow();
        }
    }
    // Infrastructures
    // TODO
}

// -----------------------------------------------------------------------------
// Name: InfrastructuresAttribute::Send
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void InfrastructuresAttribute::Send( MsgsSimToClient::MsgUrbanAttributes& message ) const
{
    for( std::map< unsigned int, ResourceNetwork >::const_iterator it = resourceMap_.begin(); it != resourceMap_.end(); ++it )
    {
        MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork* network =
            message.mutable_infrastructures()->add_resource_network();
        network->set_type( static_cast< MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_ResourceType >( it->second.type_ ) );
        network->set_enabled( it->second.enabled_ );
        if( it->second.maxStock_ )
            network->set_max_stock( it->second.maxStock_ );
        if( it->second.stock_ )
            network->set_stock( it->second.stock_ );
        if( it->second.production_ )
            network->set_production( it->second.production_ );
        if( it->second.consumption_ )
        {
            network->set_consumption( it->second.consumption_ );
            network->set_critical( it->second.critical_ );
        }
        for( std::vector< ResourceNetwork::Link >::const_iterator linkIt = it->second.links_.begin(); linkIt != it->second.links_.end(); ++linkIt )
        {
            MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link* link = network->add_link();
            link->set_kind( static_cast< MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link_TargetKind >( linkIt->kind_ ) );
            link->set_target_id( linkIt->target_ );
            link->set_capacity( linkIt->capacity_ );
            link->set_flow( linkIt->flow_ );
        }
    }
}
