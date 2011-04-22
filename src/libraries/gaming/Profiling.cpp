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
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Profiling constructor
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
Profiling::Profiling()
    : lastCall_( 0 )
    , tickSum_( 0 )
    , memory_( 0 )
    , virtualMemory_( 0 )
    , shortPathfinds_( 0 )
    , longPathfinds_( 0 )
    , actualTickDuration_( 10 )
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
void Profiling::Update( const sword::ControlProfilingInformation& message )
{
    perception_.push_back( message.perception() );
    decision_  .push_back( message.decision() );
    action_    .push_back( message.action() );
    total_     .push_back( message.main_loop() );
}

// -----------------------------------------------------------------------------
// Name: Profiling::Update
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void Profiling::Update( const sword::ControlEndTick& message )
{
    memory_ = message.memory();
    virtualMemory_ = message.virtual_memory();
    shortPathfinds_ = message.short_pathfinds();
    longPathfinds_ = message.long_pathfinds();
}

namespace
{
    const std::size_t meanCount = 50;
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
        actualTickDuration_ = tickLength > 0 ? tickLength : 9999;
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
        const float mean = float( tickSum_ ) / count;
        return 1.f / mean;
    }
    return -1.f;
}

// -----------------------------------------------------------------------------
// Name: Profiling::ActualTickDuration
// Created: SBO 2010-07-23
// -----------------------------------------------------------------------------
float Profiling::ActualTickDuration() const
{
    return actualTickDuration_;
}

// -----------------------------------------------------------------------------
// Name: Profiling::GetMemory
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
unsigned long Profiling::GetMemory() const
{
    return memory_;
}

// -----------------------------------------------------------------------------
// Name: Profiling::GetVirtualMemory
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
unsigned long Profiling::GetVirtualMemory() const
{
    return virtualMemory_;
}

// -----------------------------------------------------------------------------
// Name: Profiling::GetShortPathfinds
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
unsigned long Profiling::GetShortPathfinds() const
{
    return shortPathfinds_;
}

// -----------------------------------------------------------------------------
// Name: Profiling::GetLongPathfinds
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
unsigned long Profiling::GetLongPathfinds() const
{
    return longPathfinds_;
}
