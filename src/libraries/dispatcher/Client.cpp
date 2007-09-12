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

#include "game_asn/Asn.h"
#include "Profile.h"
#include "tools/MessageSender_ABC.h"

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
void Client::Send( const ASN1T_MsgsAfterActionReviewToClient& msg )
{
    sender_.Send( endpoint_, msg );
}
