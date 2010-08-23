// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ResourceLink.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace resource;

// -----------------------------------------------------------------------------
// Name: ResourceLink constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
ResourceLink::ResourceLink( unsigned int destination, EDestinationKind kind, int capacity )
    : destination_( destination )
    , kind_       ( kind )
    , capacity_   ( capacity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceLink constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
ResourceLink::ResourceLink( const ResourceLink& from )
    : destination_( from.destination_ )
    , kind_       ( from.kind_ )
    , capacity_   ( from.capacity_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceLink destructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
ResourceLink::~ResourceLink()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::FindDestinationKind
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
ResourceLink::EDestinationKind ResourceLink::FindDestinationKind( const std::string& kind )
{
    if( kind == "urban" )
        return eDestinationKindUrban;
    if( kind == "object" )
        return eDestinationKindObject;
    throw std::exception( "Resource link destination not valid" );
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::SetCapacity
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void ResourceLink::SetCapacity( int capacity )
{
    capacity_ = capacity;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::GetEfficientCapacity
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
int ResourceLink::GetEfficientCapacity() const
{
    // TODO apply structural/fonctional state
    return capacity_;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::GetDestination
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
unsigned int ResourceLink::GetDestination() const
{
    return destination_;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::GetDestinationKind
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
ResourceLink::EDestinationKind ResourceLink::GetDestinationKind() const
{
    return kind_;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::Serialize
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void ResourceLink::Serialize( MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link& msg ) const
{
    msg.set_kind( kind_ == eDestinationKindUrban ? 
        MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link_TargetKind_urban
        : MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link_TargetKind_object );
    msg.set_target_id( destination_ );
    msg.set_capacity( GetEfficientCapacity() );
}
