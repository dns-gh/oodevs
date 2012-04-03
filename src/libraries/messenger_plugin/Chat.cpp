// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "Chat.h"
#include "tools/MessageDispatcher_ABC.h"
#include "dispatcher/Profile_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"
#include "protocol/Protocol.h"

using namespace plugins::messenger;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Chat constructor
// Created: AGE 2008-06-10
// -----------------------------------------------------------------------------
Chat::Chat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Chat destructor
// Created: AGE 2008-06-10
// -----------------------------------------------------------------------------
Chat::~Chat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Chat::NotifyClientAuthenticated
// Created: AGE 2008-06-10
// -----------------------------------------------------------------------------
void Chat::NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile )
{
    clients_[ &client ] = &profile;
}

// -----------------------------------------------------------------------------
// Name: Chat::NotifyClientLeft
// Created: AGE 2008-06-10
// -----------------------------------------------------------------------------
void Chat::NotifyClientLeft( ClientPublisher_ABC& client )
{
    clients_.erase( &client );
}

// -----------------------------------------------------------------------------
// Name: Chat::OnReceive
// Created: AGE 2008-06-10
// -----------------------------------------------------------------------------
void Chat::OnReceive( const sword::TextMessage& message )
{
    sword::MessengerToClient answer;
    *answer.mutable_message()->mutable_text_message() = message;

    for( T_Clients::const_iterator it = clients_.begin(); it != clients_.end(); ++it )
        if( it->second->CheckRights( message.source(), message.target() ) )
            it->first->Send( answer );
}
