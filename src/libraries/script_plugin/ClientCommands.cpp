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
#include "dispatcher/ClientPublisher_ABC.h"
#include "dispatcher/LinkResolver_ABC.h"
#include "directia/Brain.h"

using namespace script;

// -----------------------------------------------------------------------------
// Name: ClientCommands constructor
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
ClientCommands::ClientCommands( dispatcher::ClientPublisher_ABC& clients, dispatcher::LinkResolver_ABC& resolver )
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

// -----------------------------------------------------------------------------
// Name: ClientCommands::RegisterIn
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void ClientCommands::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "clients", this );
    brain.RegisterFunction( "SendCommandToAll", &ClientCommands::SendCommandToAll );
    brain.RegisterFunction( "SendCommand",      &ClientCommands::SendCommand );
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
    Send( profile, command, resolver_.GetPublisher( client ) );
}

// -----------------------------------------------------------------------------
// Name: ClientCommands::Send
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void ClientCommands::Send( const std::string& profile, const std::string& command, dispatcher::ClientPublisher_ABC& target )
{
    // $$$$ AGE 2008-06-24: Faire un sender
    ASN1T_MsgsMessengerToClient answer;
    answer.t = T_MsgsMessengerToClient_msg_text_message;
    ASN1T_MsgTextMessage message;
    answer.u.msg_text_message = &message;

    message.source.profile = "script";
    message.target.profile = profile.c_str();
    message.message = command.c_str();

    target.Send( answer );
}
