// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CenterViewCommand.h"
#include "clients_gui/LinkInterpreter_ABC.h"
#include "gaming/CommandHandler.h"
#include "gaming/Command.h"

// -----------------------------------------------------------------------------
// Name: CenterViewCommand constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
CenterViewCommand::CenterViewCommand( CommandHandler& handler, gui::LinkInterpreter_ABC& interpreter )
    : handler_    ( handler )
    , interpreter_( interpreter )
{
    handler_.Register( "center", *this );
}

// -----------------------------------------------------------------------------
// Name: CenterViewCommand destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
CenterViewCommand::~CenterViewCommand()
{
    handler_.Unregister( "center", *this );
}

// -----------------------------------------------------------------------------
// Name: CenterViewCommand::Receive
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void CenterViewCommand::Receive( const Command& command )
{
    interpreter_.Interprete( command.Argument( 1 ).c_str() );
}
