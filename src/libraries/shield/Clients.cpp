// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Clients.h"
#include "Client.h"
#include "ClientHandler_ABC.h"

#pragma warning( disable: 4355 )

using namespace shield;

// -----------------------------------------------------------------------------
// Name: Clients constructor
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
Clients::Clients( tools::MessageSender_ABC& sender, tools::MessageDispatcher_ABC& dispatcher,
                  ClientHandler_ABC& handler, ClientListener_ABC& listener, bool encodeStringsInUtf8 )
    : sender_            ( sender )
    , dispatcher_        ( dispatcher )
    , handler_           ( handler )
    , listener_          ( listener )
    , utf8StringEncoding_( encodeStringsInUtf8 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Clients destructor
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
Clients::~Clients()
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        handler_.Unregister( it->first );
}

// -----------------------------------------------------------------------------
// Name: Clients::Add
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
void Clients::Add( const std::string& from )
{
    clients_.erase( from );
    boost::shared_ptr< Client > client( new Client( from, sender_, dispatcher_, handler_, listener_, utf8StringEncoding_ ) );
    clients_.insert( std::make_pair( from, client ) );
    handler_.Register( from, *client );
}

// -----------------------------------------------------------------------------
// Name: Clients::Remove
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Clients::Remove( const std::string& from )
{
    clients_.erase( from );
    handler_.Unregister( from );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToAar
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToAar( const std::string& from, const MsgsClientToAar::MsgClientToAar& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToAar( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToAuthentication
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToAuthentication( const std::string& from, const MsgsClientToAuthentication::MsgClientToAuthentication& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToAuthentication( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToMessenger
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToMessenger( const std::string& from, const MsgsClientToMessenger::MsgClientToMessenger& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToMessenger( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToReplay
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToReplay( const std::string& from, const MsgsClientToReplay::MsgClientToReplay& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToReplay( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToSim
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToSim( const std::string& from, const MsgsClientToSim::MsgClientToSim& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToSim( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveAdminToLauncher
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveAdminToLauncher( const std::string& from, const MsgsAdminToLauncher::MsgAdminToLauncher& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveAdminToLauncher( msg );
}
