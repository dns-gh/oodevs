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
#include "tools/MessageSender_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Client::Client( tools::MessageSender_ABC& sender, const std::string& endpoint )
    : sender_  ( sender )
    , endpoint_( endpoint )
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
// Name: Client::GetEndpoint
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
std::string Client::GetEndpoint() const
{
    return endpoint_;
}
