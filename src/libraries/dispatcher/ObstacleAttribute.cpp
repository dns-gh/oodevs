// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ObstacleAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( const Common::MsgObjectAttributes& message )
    : obstacle_( Common::ObstacleType_DemolitionTargetType_preliminary )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ObstacleAttribute::~ObstacleAttribute()
{
    // NOTHING
}  

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void ObstacleAttribute::Update( const Common::MsgObjectAttributes& message )
{
    if( message.has_obstacle() )
        obstacle_ = message.obstacle().type();
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ObstacleAttribute::Send( Common::MsgObjectAttributes& message ) const
{
    message.mutable_obstacle()->set_type( obstacle_ );
}
