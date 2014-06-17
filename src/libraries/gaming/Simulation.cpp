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
#include "clients_kernel/Tools.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Logger.h"
#include "Network.h"
#include "protocol/ReplaySenders.h"
#include <tuple>

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Simulation::Simulation( kernel::Controller& controller )
    : controller_ ( controller )
    , tickDuration_( 10 )
    , timeFactor_  ( 1 )
    , currentTick_ ( 0 )
    , tickCount_   ( static_cast< unsigned int >( -1 ) )
    , firstTick_   ( 1 )
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
    currentTick_ = 0;
    firstTick_ = 1;
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
    initialDate_  = message.initial_date_time().data();
    simDate_      = message.date_time().data();
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
    initialDate_  = message.initial_date_time().data();
    simDate_      = message.date_time().data();
    if( message.has_real_date_time() )
        realDate_ = message.real_date_time().data();
    if( message.has_first_tick() )
        firstTick_ = message.first_tick();
    if( message.has_end_date_time() )
        endDateTime_ = message.end_date_time().data();
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
    currentTick_ = message.current_tick();
    simDate_ = message.date_time().data();
    if( message.has_real_date_time() )
        realDate_  = message.real_date_time().data();
    profiling_.Tick();
    MT_LOG_INFO_MSG( MT_FormatString( "**** End tick %d %.2fms - %.3f MB / %.3f MB (VM)",
        currentTick_,
        1000.0*profiling_.ActualTickDuration(),
        profiling_.GetMemory() / 1048576.,
        profiling_.GetVirtualMemory() / 1048576.
        ));
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
// Created: JSR 2013-11-06
// -----------------------------------------------------------------------------
void Simulation::Update( const sword::NewDataChunkNotification& message )
{
    if( message.has_end_date_time() )
        endDateTime_ = message.end_date_time().data();
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
        checkPoint_.name_ = tools::Path::FromUTF8( message.name() );
    else
        checkPoint_.name_ = tools::Path();
    controller_.Update( checkPoint_ );
    controller_.Update( *this );
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
unsigned int Simulation::GetCurrentTick() const
{
    return currentTick_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetTickCount
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
unsigned int Simulation::GetTickCount() const
{
    return tickCount_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetFirstTick
// Created: JSR 2011-11-03
// -----------------------------------------------------------------------------
unsigned int Simulation::GetFirstTick() const
{
    return firstTick_;
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

namespace
{
    QString GetTimeAsString( const QDateTime& dateTime )
    {
        if( dateTime.isValid() )
            return dateTime.time().toString();
        return QString();
    }

    QString GetDateAsString( const QDateTime& dateTime )
    {
        if( dateTime.isValid() )
            return QLocale().toString( dateTime.date(), QLocale::LongFormat );
        return QString();
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetTimeAsString
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
QString Simulation::GetTimeAsString() const
{
    return ::GetTimeAsString( GetDateTime() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetEndDateTime
// Created: JSR 2013-11-06
// -----------------------------------------------------------------------------
QDateTime Simulation::GetEndDateTime() const
{
    return tools::IsoStringToQDateTime( endDateTime_ );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetDateAsString
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
QString Simulation::GetDateAsString() const
{
    return ::GetDateAsString( GetDateTime() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetDateTime
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
QDateTime Simulation::GetDateTime() const
{
    return tools::IsoStringToQDateTime( simDate_ );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetRealDateTime
// Created: JSR 2013-10-31
// -----------------------------------------------------------------------------
QDateTime Simulation::GetRealDateTime() const
{
    QDateTime date = tools::IsoStringToQDateTime( realDate_ );
    date.setTimeSpec( Qt::UTC );
    return date.toLocalTime();
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetRealTimeAsString
// Created: JSR 2013-10-31
// -----------------------------------------------------------------------------
QString Simulation::GetRealTimeAsString() const
{
    return ::GetTimeAsString( GetRealDateTime() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetRealDateAsString
// Created: JSR 2013-10-31
// -----------------------------------------------------------------------------
QString Simulation::GetRealDateAsString() const
{
    return ::GetDateAsString( GetRealDateTime() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetInitialDateTime
// Created: SBO 2008-04-24
// -----------------------------------------------------------------------------
QDateTime Simulation::GetInitialDateTime() const
{
    return tools::IsoStringToQDateTime( initialDate_ );
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

void Simulation::Update( const sword::Timeskip& message )
{
    skips_.insert( std::make_pair( message.tick(), tools::GDHStringToQDateTime( message.time() ).toTime_t() ) );
}

QDateTime Simulation::GetTime( uint32_t tick ) const
{
    uint32_t max = 0, time = 0;
    for( auto it = skips_.begin(); it != skips_.end(); ++it )
        if( it->first > tick )
            break;
        else
            std::tie( max, time ) = *it;
    if( !max )
        return QDateTime();
    auto now = QDateTime::fromTime_t( time + ( tick - max ) * tickDuration_ );
    now.setTimeSpec( Qt::LocalTime );
    return now.toLocalTime();
}