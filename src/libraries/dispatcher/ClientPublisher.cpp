// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ClientPublisher.h"
#include "Client.h"
#include "DIN/MessageService/DIN_BufferedMessage.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ClientPublisher constructor
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
ClientPublisher::ClientPublisher( Client& client )
    : Publisher_ABC()
    , client_      ( client )
{

}

// -----------------------------------------------------------------------------
// Name: ClientPublisher destructor
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
ClientPublisher::~ClientPublisher()
{

}

// -----------------------------------------------------------------------------
// Name: ClientPublisher::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ClientPublisher::Send( const ASN1T_MsgsInSim& /*msg*/ )
{
    throw std::runtime_error( "Invalid publisher" );
}

// -----------------------------------------------------------------------------
// Name: ClientPublisher::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ClientPublisher::Send( const ASN1T_MsgsInClient& msg )
{
    client_.Send( msg );
}

// -----------------------------------------------------------------------------
// Name: ClientPublisher::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ClientPublisher::Send( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg )
{
    client_.Send( nMsgID, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: ClientPublisher::BuildDinMsg
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
DIN::DIN_BufferedMessage ClientPublisher::BuildDinMsg()
{
    return client_.BuildDinMsg();
}