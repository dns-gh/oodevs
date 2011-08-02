// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LauncherCommand.h"
#include "gaming/Command.h"
#include "gaming/CommandHandler.h"
#include "tools/ExerciseConfig.h"
#include "clients_gui/LinkInterpreter_ABC.h"

// -----------------------------------------------------------------------------
// Name: LauncherCommand constructor
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
LauncherCommand::LauncherCommand( CommandHandler& handler, tools::ExerciseConfig& config, gui::LinkInterpreter_ABC& interpreter )
    : handler_( handler )
    , config_ ( config )
    , interpreter_( interpreter )
{
    handler_.Register( "launch", *this );
}

// -----------------------------------------------------------------------------
// Name: LauncherCommand destructor
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
LauncherCommand::~LauncherCommand()
{
    handler_.Unregister( "launch", *this );
}

// -----------------------------------------------------------------------------
// Name: LauncherCommand::Receive
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
void LauncherCommand::Receive( const Command& command )
{
    QProcess process;
    process.start( config_.BuildExerciseChildFile( command.Argument( 1 ) ).c_str() ) ;
}
