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
ObstacleAttribute::ObstacleAttribute( const sword::ObjectAttributes& message )
    : obstacle_      ( sword::ObstacleType_DemolitionTargetType_preliminary )
    , isActivated_   ( true )
    , activationTime_( 0 )
    , activityTime_  ( 0 )
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
void ObstacleAttribute::Update( const sword::ObjectAttributes& message )
{
    if( message.has_obstacle() )
    {
        obstacle_ = message.obstacle().type();
        if( message.obstacle().has_activated() )
            isActivated_ = message.obstacle().activated();
        if( message.obstacle().has_activation_time() )
            activationTime_ = message.obstacle().activation_time();
        if( message.obstacle().has_activity_time() )
            activityTime_ = message.obstacle().activity_time();
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ObstacleAttribute::Send( sword::ObjectAttributes& message ) const
{
    message.mutable_obstacle()->set_type( obstacle_ );
    message.mutable_obstacle()->set_activated( isActivated_ );
    message.mutable_obstacle()->set_activation_time( activationTime_ );
    message.mutable_obstacle()->set_activity_time( activityTime_ );
}
