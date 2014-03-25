// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ClientCommands.h"
#include "protocol/ClientPublisher_ABC.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "protocol/MessengerSenders.h"
#include <directia/brain/Brain.h>

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ClientCommands constructor
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
ClientCommands::ClientCommands( dispatcher::ClientPublisher_ABC& clients, const dispatcher::LinkResolver_ABC& resolver )
    : clients_( clients )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientCommands destructor
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
ClientCommands::~ClientCommands()
{
    // NOTHING
}

namespace directia
{
    void UsedByDIA( ClientCommands* ) {}
    void ReleasedByDIA( ClientCommands* ) {}
}

// -----------------------------------------------------------------------------
// Name: ClientCommands::RegisterIn
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void ClientCommands::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "clients" ] = this;
    brain.Register( "SendCommandToAll", &ClientCommands::SendCommandToAll );
    brain.Register( "SendCommand",      &ClientCommands::SendCommand );
}

// -----------------------------------------------------------------------------
// Name: ClientCommands::SendCommandToAll
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void ClientCommands::SendCommandToAll( const std::string& profile, const std::string& command )
{
    Send( profile, command, clients_ );
}

// -----------------------------------------------------------------------------
// Name: ClientCommands::SendCommand
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void ClientCommands::SendCommand( const std::string& client, const std::string& profile, const std::string& command )
{
    Send( profile, command, resolver_.GetAuthenticatedPublisher( client ) );
}

// -----------------------------------------------------------------------------
// Name: ClientCommands::Send
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void ClientCommands::Send( const std::string& profile, const std::string& command, dispatcher::ClientPublisher_ABC& target )
{
    sword::MessengerToClient answer;
    answer.mutable_message()->mutable_text_message()->mutable_source()->set_profile("script");
    answer.mutable_message()->mutable_text_message()->mutable_target()->set_profile(profile.c_str());
    answer.mutable_message()->mutable_text_message()->set_message(command.c_str());
    target.Send( answer );
}
