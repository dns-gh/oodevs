// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Client.h"
#include "protocol/Protocol.h"
#include "protocol/ClientBroadcaster_ABC.h"
#include "tools/MessageSender_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Client::Client( tools::MessageSender_ABC& sender, ClientBroadcaster_ABC& broadcaster, const std::string& endpoint )
    : sender_     ( sender )
    , broadcaster_( broadcaster )
    , endpoint_   ( endpoint )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Client destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Client::~Client()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client::Activate
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void Client::Activate()
{
    broadcaster_.Activate( endpoint_ );
}

// -----------------------------------------------------------------------------
// Name: Client::Deactivate
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void Client::Deactivate()
{
    broadcaster_.Deactivate( endpoint_ );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Client::Send( const sword::AuthenticationToClient& msg )
{
    sender_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Client::Send( const sword::SimToClient& msg )
{
    sender_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void Client::Send( const sword::ReplayToClient& msg )
{
    sender_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Client::Send( const sword::AarToClient& msg )
{
    sender_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
void Client::Send( const sword::MessengerToClient& msg )
{
    sender_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Client::Send( const sword::DispatcherToClient& msg )
{
    sender_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void Client::Send( unsigned long tag, tools::Message& message ) const
{
    sender_.Send( endpoint_, tag, message );
}
