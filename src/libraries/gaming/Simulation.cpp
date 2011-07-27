// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Simulation.h"
#include "clients_kernel/Controller.h"
#include "Network.h"
#include "protocol/ReplaySenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Simulation::Simulation( Controller& controller )
    : controller_  ( controller )
    , tickDuration_( 10 )
    , timeFactor_  ( 1 )
    , currentTick_ ( 0 )
    , tickCount_   ( static_cast< unsigned int >( -1 ) )
    , time_        ( 0 )
    , paused_      ( false )
    , connected_   ( false )
    , initialized_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Simulation::~Simulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation::Connect
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Simulation::Connect( const std::string& host )
{
    profiling_.Clear();
    connected_ = true;
    simulationHost_ = host;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Disconnect
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Simulation::Disconnect()
{
    if( !connected_ )
        return;
    time_ = 0;
    currentTick_ = 0;
    profiling_.Clear();
    connected_ = false;
    initialized_ = false;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Pause
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Pause( bool paused )
{
    profiling_.Clear();
    paused_ = paused;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::ChangeSpeed
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::ChangeSpeed( int timeFactor )
{
    timeFactor_ = timeFactor;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::ControlInformation& message )
{
    tickCount_    = static_cast< unsigned int >( -1 );
    currentTick_  = message.current_tick();
    tickDuration_ = message.tick_duration();
    paused_       = message.status() == sword::paused;
    timeFactor_   = message.time_factor();
    time_         = message.current_tick() * tickDuration_;
    initialDate_  = message.initial_date_time().data();
    date_         = message.date_time().data();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::ControlReplayInformation& message )
{
    tickDuration_ = message.tick_duration();
    currentTick_  = message.current_tick();
    tickCount_    = message.tick_count();
    paused_       = message.status() == sword::paused;
    timeFactor_   = message.time_factor();
    time_         = message.current_tick() * tickDuration_;
    initialDate_  = message.initial_date_time().data();
    date_         = message.date_time().data();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::ControlProfilingInformation& message )
{
    profiling_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::ControlBeginTick& message )
{
    int tick = message.current_tick();
    date_ = message.date_time().data();
    profiling_.Tick();
    currentTick_ = tick;
    time_ = tick * tickDuration_;
    controller_.Update( startTick_ );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::ControlEndTick& message )
{
    profiling_.Update( message );
    controller_.Update( endTick_ );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: JSR 2010-05-12
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::ControlSendCurrentStateEnd& /*message*/ )
{
    initialized_ = true;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::TimeTable& message )
{
    controller_.Update( sTimeTable( message ) );
}

// -----------------------------------------------------------------------------
// Name: Simulation::BeginCheckPoint
// Created: SBO 2007-03-09
// -----------------------------------------------------------------------------
void Simulation::BeginCheckPoint()
{
    checkPoint_.start_ = true;
    controller_.Update( checkPoint_ );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::EndCheckPoint
// Created: SBO 2007-03-09
// -----------------------------------------------------------------------------
void Simulation::EndCheckPoint( const sword::ControlCheckPointSaveEnd& message )
{
    checkPoint_.start_ = false;
    if( message.has_name() )
        checkPoint_.name_ = message.name();
    else
        checkPoint_.name_ = "";
    controller_.Update( checkPoint_ );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetTime
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
int Simulation::GetTime() const
{
    return time_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::IsPaused
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
bool Simulation::IsPaused() const
{
    return paused_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::IsConnected
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
bool Simulation::IsConnected() const
{
    return connected_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::IsInitialized
// Created: JSR 2010-05-12
// -----------------------------------------------------------------------------
bool Simulation::IsInitialized() const
{
    return initialized_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetSpeed
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
int Simulation::GetSpeed() const
{
    return timeFactor_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetCurrentTick
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
unsigned Simulation::GetCurrentTick() const
{
    return currentTick_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetTickCount
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
unsigned Simulation::GetTickCount() const
{
    return tickCount_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetTickDuration
// Created: SBO 2008-04-29
// -----------------------------------------------------------------------------
unsigned int Simulation::GetTickDuration() const
{
    return tickDuration_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetSimulationHost
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
const std::string& Simulation::GetSimulationHost() const
{
    return simulationHost_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetEffectiveSpeed
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
float Simulation::GetEffectiveSpeed() const
{
    return std::floor( ( profiling_.EffectiveSpeed() + 0.5f ) * tickDuration_ );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetActualSpeed
// Created: SBO 2010-07-23
// -----------------------------------------------------------------------------
float Simulation::GetActualSpeed() const
{
    return std::floor( tickDuration_ / profiling_.ActualTickDuration() + 0.5f );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetTimeAsString
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
QString Simulation::GetTimeAsString() const
{
    return GetDateTime().time().toString();
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetDateAsString
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
QString Simulation::GetDateAsString() const
{
    return GetDateTime().date().toString();
}

namespace
{
    QDateTime MakeDate( const std::string& str )
    {
        // $$$$ AGE 2007-10-12: ...
        QString extended( str.c_str() );
        extended.insert( 13, ':' ); extended.insert( 11, ':' );
        extended.insert(  6, '-' ); extended.insert(  4, '-' );
        return QDateTime::fromString( extended, Qt::ISODate );
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetDateTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
QDateTime Simulation::GetDateTime() const
{
    return MakeDate( date_ );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetInitialDateTime
// Created: SBO 2008-04-24
// -----------------------------------------------------------------------------
QDateTime Simulation::GetInitialDateTime() const
{
    return MakeDate( initialDate_ );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetMemory
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
unsigned long Simulation::GetMemory() const
{
    return profiling_.GetMemory();
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetVirtualMemory
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
unsigned long Simulation::GetVirtualMemory() const
{
    return profiling_.GetVirtualMemory();
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetShortPathfinds
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
unsigned long Simulation::GetShortPathfinds() const
{
    return profiling_.GetShortPathfinds();
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetLongPathfinds
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
unsigned long Simulation::GetLongPathfinds() const
{
    return profiling_.GetLongPathfinds();
}
