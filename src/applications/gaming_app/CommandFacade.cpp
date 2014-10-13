// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CommandFacade.h"
#include "CenterViewCommand.h"
#include "ZoomViewCommand.h"
#include "ChangeOptionCommand.h"
#include "DockCommand.h"
#include "LauncherCommand.h"
#include "FilterViewCommand.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: CommandFacade constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
CommandFacade::CommandFacade( QMainWindow* parent, kernel::Controllers& controllers, tools::ExerciseConfig& config,
                              CommandHandler& handler, gui::LinkInterpreter_ABC& interpreter, gui::GLView_ABC& view,
                              ProfileFilter& profile, const kernel::CoordinateConverter_ABC& converter )
    : QObject( parent )
{
    Add( new CenterViewCommand  ( handler, interpreter ) );
    Add( new ZoomViewCommand    ( handler, view, converter ) );
    Add( new ChangeOptionCommand( handler, controllers.options_ ) );
    Add( new DockCommand        ( handler, *parent ) );
    Add( new LauncherCommand    ( handler, config, interpreter ) );
    Add( new FilterViewCommand  ( handler, profile ) );
}

// -----------------------------------------------------------------------------
// Name: CommandFacade destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
CommandFacade::~CommandFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommandFacade::Add
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void CommandFacade::Add( CommandHandler_ABC* h )
{
    handlers_.push_back( h );
}
