// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ClientPublisher_ABC constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ClientPublisher_ABC::ClientPublisher_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientPublisher_ABC destructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ClientPublisher_ABC::~ClientPublisher_ABC()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ClientPublisher_ABC::Send
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ClientPublisher_ABC::Send( const ASN1T_MsgsSimToClient& )
{
    throw std::runtime_error( "Invalid dispatcher : " __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: ClientPublisher_ABC::Send
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ClientPublisher_ABC::Send( const ASN1T_MsgsAuthenticationToClient& )
{
    throw std::runtime_error( "Invalid dispatcher : " __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: ClientPublisher_ABC::Send
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ClientPublisher_ABC::Send( const ASN1T_MsgsReplayToClient& )
{
    throw std::runtime_error( "Invalid dispatcher : " __FUNCTION__ " not implemented" );
}
