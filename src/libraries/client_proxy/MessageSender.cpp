// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "client_proxy_pch.h"
#include "MessageSender.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/SimulationSenders.h"
#include "protocol/MessengerSenders.h"
#include "tools/MessageSender_ABC.h"

// -----------------------------------------------------------------------------
// Name: MessageSender constructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
MessageSender::MessageSender( tools::MessageSender_ABC& sender, const std::string& endpoint )
    : sender_( sender )
    , endpoint_( endpoint )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessageSender destructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
MessageSender::~MessageSender()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessageSender::Send
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void MessageSender::Send( const sword::ClientToAuthentication& message )
{
    sender_.Send( endpoint_, message );
}

// -----------------------------------------------------------------------------
// Name: MessageSender::Send
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void MessageSender::Send( const sword::ClientToSim& message )
{
    sender_.Send( endpoint_, message );
}

// -----------------------------------------------------------------------------
// Name: MessageSender::Send
// Created: LGY 2011-11-09
// -----------------------------------------------------------------------------
void MessageSender::Send( const sword::ClientToMessenger& message )
{
    sender_.Send( endpoint_, message );
}
