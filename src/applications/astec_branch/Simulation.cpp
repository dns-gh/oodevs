// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Simulation.h"
#include "App.h"

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Simulation::Simulation( App& application )
    : application_( application )
    , tickDuration_( 10 )
    , time_( 0 )
    , paused_( false )
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
// Name: Simulation::Pause
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Pause( bool paused )
{
    paused_ = paused;
    application_.NotifyPauseStatusChanged( paused );
}

// -----------------------------------------------------------------------------
// Name: Simulation::ChangeSpeed
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::ChangeSpeed( int timeFactor )
{
    MT_LOG_INFO( "Facteur temps: " << timeFactor, eReceived, 0 );
    application_.NotifySpeedChanged( timeFactor );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Update( const ASN1T_MsgCtrlInfo& asnMsg )
{
    MT_LOG_INFO( "CtrlInfo - Current Tick: " << asnMsg.current_tick
                 << " - Tick duration : "       << asnMsg.tick_duration
                 << " - Time factor: "          << asnMsg.time_factor
                 << " - Exercice ID: "          << asnMsg.id_exercice
                 << " - Checkpoint Frequency: " << asnMsg.checkpoint_frequence,
                 eReceived, 0 );
    tickDuration_ = asnMsg.tick_duration;
    ChangeSpeed( asnMsg.time_factor );
}

// -----------------------------------------------------------------------------
// Name: Simulation::BeginTick
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::BeginTick( int tick )
{
    time_ = tick * tickDuration_;
    application_.NotifyTimeChanged( time_ );
    application_.NotifyTickStartEnd( true );
}

// -----------------------------------------------------------------------------
// Name: Simulation::EndTick
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::EndTick()
{
    application_.NotifyTickStartEnd( false );
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
