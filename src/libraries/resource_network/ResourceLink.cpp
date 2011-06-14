// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ResourceLink.h"
#include "protocol/Protocol.h"
#include "ResourceTools_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace resource;

// -----------------------------------------------------------------------------
// Name: ResourceLink constructor
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
ResourceLink::ResourceLink()
    : target_    ( 0 )
    , kind_      ( eTargetKindUrban )
    , capacity_  ( 0 )
    , flow_      ( 0 )
    , oldFlow_   ( 0 )
    , needUpdate_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceLink constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
ResourceLink::ResourceLink( unsigned int target, ETargetKind kind, int capacity )
    : target_    ( target )
    , kind_      ( kind )
    , capacity_  ( capacity )
    , flow_      ( 0 )
    , oldFlow_   ( 0 )
    , needUpdate_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceLink constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
ResourceLink::ResourceLink( const ResourceLink& from )
    : target_    ( from.target_ )
    , kind_      ( from.kind_ )
    , capacity_  ( from.capacity_ )
    , flow_      ( from.flow_ )
    , oldFlow_   ( 0 )
    , needUpdate_( true )
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
// Name: ResourceLink::Finalize
// Created: JSR 2011-01-17
// -----------------------------------------------------------------------------
void ResourceLink::Finalize( const ResourceTools_ABC& tools )
{
    if( kind_ == eTargetKindUrban )
        target_ = tools.GetIdFromSimulation( target_ );
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
    if( capacity != capacity_ )
        needUpdate_ = true;
    capacity_ = capacity;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::GetEfficientCapacity
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
int ResourceLink::GetEfficientCapacity() const
{
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
// Name: ResourceLink::ResetFlow
// Created: JSR 2010-11-30
// -----------------------------------------------------------------------------
void ResourceLink::ResetFlow()
{
    oldFlow_ = flow_;
    flow_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::SetFlow
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void ResourceLink::SetFlow( unsigned int flow )
{
    if( flow != oldFlow_ )
    {
        needUpdate_ = true;
        oldFlow_ = flow;
    }
    flow_ = flow;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::NeedUpdate
// Created: JSR 2010-11-30
// -----------------------------------------------------------------------------
bool ResourceLink::NeedUpdate() const
{
    return needUpdate_;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::Serialize
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void ResourceLink::Serialize( sword::ResourceNetwork_Link& msg ) const
{
    msg.mutable_object()->set_id( target_ );
    msg.set_capacity( capacity_ );
    msg.set_flow( flow_ );
    needUpdate_ = false;
}
