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

#include "game_asn/Simulation.h"
#include "Profile.h"
#include "tools/MessageSender_ABC.h"
#include "game_asn/ClientSenders.h"
#include "game_asn/AuthenticationSenders.h"
#include "game_asn/ReplaySenders.h"
#include "game_asn/AarSenders.h"
#include "game_asn/MessengerSenders.h"
#include "game_asn/DispatcherSenders.h"

using namespace dispatcher;
using namespace tools;

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
void Client::Send( const ASN1T_MsgsAuthenticationToClient& asnMsg )
{
    sender_.Send( endpoint_, asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsSimToClient& asnMsg )
{
    sender_.Send( endpoint_, asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsReplayToClient& msg )
{
    sender_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsAarToClient& msg )
{
    sender_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsMessengerToClient& msg )
{
    sender_.Send( endpoint_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsDispatcherToClient& msg )
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
