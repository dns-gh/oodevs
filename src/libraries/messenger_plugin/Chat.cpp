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
#include "dispatcher/ClientPublisher_ABC.h"

using namespace messenger;
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
void Chat::OnReceive( const ASN1T_MsgTextMessage& message )
{
    ASN1T_MsgsMessengerToClient answer;
    answer.t = T_MsgsMessengerToClient_msg_text_message;
    answer.u.msg_text_message = const_cast< ASN1T_MsgTextMessage* >( &message );

    for( T_Clients::const_iterator it = clients_.begin(); it != clients_.end(); ++it )
        if( it->second->CheckRights( message.source, message.target ) )
            it->first->Send( answer );
}
