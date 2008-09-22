// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "CommandHandler.h"
#include "CommandHandler_ABC.h"
#include "Command.h"

// -----------------------------------------------------------------------------
// Name: CommandHandler constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
CommandHandler::CommandHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommandHandler destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
CommandHandler::~CommandHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommandHandler::Register
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void CommandHandler::Register( const std::string& command, CommandHandler_ABC& handler )
{
    handlers_.insert( std::make_pair( command, &handler ) );
}

// -----------------------------------------------------------------------------
// Name: CommandHandler::Unregister
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void CommandHandler::Unregister( const std::string& command, CommandHandler_ABC& handler )
{
    std::pair< IT_Handlers, IT_Handlers > its = handlers_.equal_range( command );
    IT_Handlers it = std::find( its.first, its.second, T_NamedHandler( command, &handler ) );
    if( it != its.second )
        handlers_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: CommandHandler::Receive
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void CommandHandler::Receive( const std::string& from, const std::string& to, const std::string& text )
{
    Command command( from, to, text );
    std::pair< IT_Handlers, IT_Handlers > its = handlers_.equal_range( command.Name() );
    for( IT_Handlers it = its.first; it != its.second; ++it )
        it->second->Receive( command );
}
