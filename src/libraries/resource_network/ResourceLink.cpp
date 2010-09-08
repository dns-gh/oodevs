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
ResourceLink::ResourceLink( unsigned int target, ETargetKind kind, int capacity )
    : target_( target )
    , kind_       ( kind )
    , capacity_   ( capacity )
    , flow_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceLink constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
ResourceLink::ResourceLink( const ResourceLink& from )
    : target_( from.target_ )
    , kind_       ( from.kind_ )
    , capacity_   ( from.capacity_ )
    , flow_       ( from.flow_ )
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
// Name: ResourceLink::FindTargetKind
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
ResourceLink::ETargetKind ResourceLink::FindTargetKind( const std::string& kind )
{
    if( kind == "urban-object" )
        return eTargetKindUrban;
    if( kind == "terrain-object" )
        return eTargetKindObject;
    throw std::exception( "Resource link target not valid" );
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
    if( capacity_ == -1 )
        return -1;
    // TODO apply structural/fonctional state
    return capacity_;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::GetTarget
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
unsigned int ResourceLink::GetTarget() const
{
    return target_;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::GetTargetKind
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
ResourceLink::ETargetKind ResourceLink::GetTargetKind() const
{
    return kind_;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::SetFlow
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void ResourceLink::SetFlow( unsigned int flow )
{
    flow_ = flow;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::Serialize
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void ResourceLink::Serialize( Common::ResourceNetwork_Link& msg ) const
{
    msg.set_kind( kind_ == eTargetKindUrban ? 
        Common::ResourceNetwork_Link_TargetKind_urban : Common::ResourceNetwork_Link_TargetKind_object );
    msg.set_target_id( target_ );
    msg.set_capacity( capacity_ );
    msg.set_flow( flow_ );
}
