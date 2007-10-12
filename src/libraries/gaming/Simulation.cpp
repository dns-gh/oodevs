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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Simulation::Simulation( Controller& controller )
    : controller_( controller )
    , tickDuration_( 10 )
    , timeFactor_( 1 )
    , tickCount_( unsigned( -1 ) )
    , time_( 0 )
    , paused_( false )
    , connected_( false )
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
    connection_.connected_ = connected_;
    controller_.Update( connection_ );
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
    profiling_.Clear();
    connected_ = false;
    connection_.connected_ = connected_;
    controller_.Update( connection_ );
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
void Simulation::Update( const ASN1T_MsgControlInformation& message )
{
    tickCount_    = unsigned( -1 );
    tickDuration_ = message.tick_duration;
    paused_       = message.status == EnumSimulationState::paused;
    timeFactor_   = message.time_factor;
    time_         = message.current_tick * tickDuration_;
    date_         = std::string( (const char*)message.date_time.data, 15 );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void Simulation::Update( const ASN1T_MsgControlReplayInformation& message )
{
    tickDuration_ = message.tick_duration;
    tickCount_    = message.tick_count;
    paused_       = message.status == EnumSimulationState::paused;
    timeFactor_   = message.time_factor;
    time_         = message.current_tick * tickDuration_;
    date_         = std::string( (const char*)message.date_time.data, 15 );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void Simulation::Update( const ASN1T_MsgControlProfilingInformation& message )
{
    profiling_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Update( const ASN1T_MsgControlBeginTick& message )
{
    int tick = message.current_tick;
    date_ = std::string( (const char*)message.date_time.data, 15 );
    profiling_.Tick();
    time_ = tick * tickDuration_;
    controller_.Update( startTick_ );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Update( const ASN1T_MsgControlEndTick& message )
{
    profiling_.Update( message );
    controller_.Update( endTick_ );
    controller_.Update( *this );
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
void Simulation::EndCheckPoint()
{
    checkPoint_.start_ = false;
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
    return tickDuration_ ? time_ / tickDuration_ : 0;
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

// -----------------------------------------------------------------------------
// Name: Simulation::GetDateTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
QDateTime Simulation::GetDateTime() const
{
    // $$$$ AGE 2007-10-12: ...
    QString extended( date_.c_str() );
    extended.insert( 13, ':' ); extended.insert( 11, ':' ); 
    extended.insert(  6, '-' ); extended.insert(  4, '-' );
    return QDateTime::fromString( extended, Qt::ISODate );
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

// -----------------------------------------------------------------------------
// Name: Simulation::IsReplayer
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
bool Simulation::IsReplayer() const
{
    return GetTickCount() != unsigned( -1 );
}
