// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "SimulationCommands.h"
#include <directia/Brain.h>
#include "dispatcher/SimulationPublisher_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <MT/MT_Logger/MT_Logger_lib.h>
#include "protocol/simulationsenders.h"

using namespace plugins::script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationCommands constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
SimulationCommands::SimulationCommands( dispatcher::SimulationPublisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : publisher_( publisher )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationCommands destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
SimulationCommands::~SimulationCommands()
{
    // NOTHING
}

namespace
{
    void Trace( const std::string& message )
    {
        MT_LOG_INFO_MSG( message );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationCommands::RegisterIn
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void SimulationCommands::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject  ( "sim", this );
    brain.RegisterFunction( "Pause", &SimulationCommands::Pause );
    brain.RegisterFunction( "Resume", &SimulationCommands::Resume );
    brain.RegisterFunction( "ChangeTimeFactor", &SimulationCommands::ChangeTimeFactor );
    brain.RegisterFunction( "CreateUnit", &SimulationCommands::CreateUnit );
    brain.RegisterFunction( "Engage", &SimulationCommands::Engage );
    brain.RegisterFunction( "Trace", &::Trace );
}

// -----------------------------------------------------------------------------
// Name: SimulationCommands::Pause
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void SimulationCommands::Pause()
{
    simulation::ControlPause pause;
    pause.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: SimulationCommands::Resume
// Created: SBO 2008-06-25
// -----------------------------------------------------------------------------
void SimulationCommands::Resume()
{
    simulation::ControlResume resume;
    resume.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: SimulationCommands::ChangeTimeFactor
// Created: SBO 2008-06-25
// -----------------------------------------------------------------------------
void SimulationCommands::ChangeTimeFactor( unsigned int factor )
{
    simulation::ControlChangeTimeFactor changeFactor;
    changeFactor().set_time_factor( factor );
    changeFactor.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: SimulationCommands::CreateUnit
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
void SimulationCommands::CreateUnit( const Position& position, unsigned int type, unsigned int parent )
{
    simulation::UnitCreationRequest request;
    request().mutable_type()->set_id( type );
    request().mutable_superior()->set_id( parent );
    converter_.ConvertToGeo( ToPoint( position ), *request().mutable_position() );
    request.Send( publisher_, 0 );
}

// -----------------------------------------------------------------------------
// Name: SimulationCommands::Engage
// Created: AGE 2008-08-01
// -----------------------------------------------------------------------------
void SimulationCommands::Engage( unsigned automat, bool engage )
{
    simulation::SetAutomatMode mode;
    mode().mutable_automate()->set_id( automat );
    mode().set_mode( engage ? Common::embraye : Common::debraye );
    mode.Send( publisher_,0 );
}
