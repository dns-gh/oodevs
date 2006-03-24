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
#include "Controllers.h"
#include "Controller.h"
#include "Network.h"

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Simulation::Simulation( Controllers& controllers )
    : controllers_( controllers )
    , tickDuration_( 10 )
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
void Simulation::Connect()
{
    connected_ = true;
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Disconnect
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Simulation::Disconnect()
{
    connected_ = false;
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Pause
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Pause( bool paused )
{
    paused_ = paused;
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::ChangeSpeed
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::ChangeSpeed( int timeFactor )
{
    timeFactor_ = timeFactor;
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::Update( const ASN1T_MsgCtrlInfo& message )
{
    tickDuration_ = message.tick_duration;
    paused_       = message.etat == EnumEtatSim::paused;
    timeFactor_   = message.time_factor;
    time_         = message.current_tick * tickDuration_;
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::BeginTick
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::BeginTick( int tick )
{
    time_ = tick * tickDuration_;
    tickStart_ = true; // $$$$ AGE 2006-02-14: 
//    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Simulation::EndTick
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void Simulation::EndTick()
{
    tickStart_ = false;
//    controllers_.controller_.Update( *this );
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
