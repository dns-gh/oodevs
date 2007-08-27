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
#include "tools/ObjectMessageService.h"
#include "DIN/DIN_Link.h"

using namespace dispatcher;
using namespace tools;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Client::Client( ObjectMessageService& messageService, DIN::DIN_Link& link )
    : Client_ABC ( messageService, link )
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
    messageService_.Send( link_, asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsSimToClient& asnMsg )
{
    messageService_.Send( link_, asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsReplayToClient& msg )
{
    messageService_.Send( link_, msg );
}

// -----------------------------------------------------------------------------
// Name: Client::GetClientFromLink
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
// static
Client& Client::GetClientFromLink( const DIN_Link& link )
{
    DIN_UserData_ABC* pTmp = link.GetUserData();
    assert( pTmp );
    return *static_cast< Client* >( pTmp );    
}
