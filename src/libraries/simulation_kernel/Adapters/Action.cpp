// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Action.h"
#include "Sink.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_Entity_ABC.h"
#include <core/Model.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: Action constructor
// Created: MCO 2012-10-10
// -----------------------------------------------------------------------------
Action::Action( Sink& sink, MIL_Entity_ABC& entity, const std::string& name, core::Model& parameters )
    : sink_    ( sink )
    , decision_( entity.GetRole< DEC_Decision_ABC >() )
{
    parameters[ "action" ] = GetId();
    command_ = sink.StartCommand( name, parameters );
}

// -----------------------------------------------------------------------------
// Name: Action destructor
// Created: MCO 2012-10-10
// -----------------------------------------------------------------------------
Action::~Action()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action::Suspend
// Created: MCO 2012-10-10
// -----------------------------------------------------------------------------
void Action::Suspend()
{
    sink_.PauseCommand( command_ );
}

// -----------------------------------------------------------------------------
// Name: Action::Resume
// Created: MCO 2012-10-10
// -----------------------------------------------------------------------------
void Action::Resume()
{
    sink_.ResumeCommand( command_ );
}

// -----------------------------------------------------------------------------
// Name: Action::Stop
// Created: MCO 2012-10-10
// -----------------------------------------------------------------------------
void Action::Stop()
{
    sink_.StopCommand( command_ );
    decision_.RemoveCallback( GetId() );
}

// -----------------------------------------------------------------------------
// Name: Action::Execute
// Created: MCO 2012-10-10
// -----------------------------------------------------------------------------
void Action::Execute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action::ExecuteSuspended
// Created: MCO 2012-10-10
// -----------------------------------------------------------------------------
void Action::ExecuteSuspended()
{
    // NOTHING
}
