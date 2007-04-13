// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Profiling.h"
#include "DIN_Types.h"

// -----------------------------------------------------------------------------
// Name: Profiling constructor
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
Profiling::Profiling()
    : lastCall_( 0 )
    , tickSum_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profiling destructor
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
Profiling::~Profiling()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profiling::Update
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void Profiling::Update( const ProfilingValuesMessage& message )
{
    double rPerceptionTime, rDecisionTime, rActionTime, rMainLoopTime;
    message >> rPerceptionTime >> rDecisionTime >> rActionTime >> rMainLoopTime;
    perception_.push_back( rPerceptionTime );
    decision_  .push_back( rDecisionTime );
    action_    .push_back( rActionTime );
    total_     .push_back( rMainLoopTime );
}

namespace
{
    const unsigned meanCount = 50;
}

// -----------------------------------------------------------------------------
// Name: Profiling::Tick
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Profiling::Tick()
{
    clock_t now = clock();
    if( lastCall_ )
    {
        const float tickLength = float( now - lastCall_ ) / float( CLOCKS_PER_SEC );
        ticks_.push_back( tickLength );
        tickSum_ += tickLength;
        if( ticks_.size() > meanCount )
            tickSum_ -= ticks_[ ticks_.size() - meanCount ];
    }
    lastCall_ = now;
}
    
// -----------------------------------------------------------------------------
// Name: Profiling::Clear
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Profiling::Clear()
{
    tickSum_ = 0;
    ticks_.resize( 0 );
}

// -----------------------------------------------------------------------------
// Name: Profiling::EffectiveSpeed
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
float Profiling::EffectiveSpeed() const
{
    unsigned count = std::min( meanCount, ticks_.size() );
    if( count )
    {
        const float mean = tickSum_ / count;
        return 1.f / mean;
    }
    return -1.f;
}
