// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "SocketManager.h"
#include "Socket.h"
#include "ConnectionCallback_ABC.h"
#include <boost/lexical_cast.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: SocketManager constructor
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
SocketManager::SocketManager( MessageCallback_ABC& message, ConnectionCallback_ABC& connection )
    : message_   ( message )
    , connection_( connection )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SocketManager destructor
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
SocketManager::~SocketManager()
{
    try
    {
        Disconnect();
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: SocketManager::Disconnect
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void SocketManager::Disconnect()
{
    while( !sockets_.empty() )
    {
        sockets_.begin()->second->Close();
        sockets_.erase( sockets_.begin() );
    }
}

// -----------------------------------------------------------------------------
// Name: SocketManager::Disconnect
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void SocketManager::Disconnect( const std::string& endpoint )
{
    IT_Sockets it = sockets_.find( endpoint );
    if( it != sockets_.end() )
    {
        it->second->Close();
        sockets_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: SocketManager::Add
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void SocketManager::Add( const boost::shared_ptr< boost::asio::ip::tcp::socket >& socket )
{
    const std::string address = ToString( socket->remote_endpoint() );
    boost::shared_ptr< Socket > pSocket( new Socket( socket, address, message_ ) );
    pSocket->StartReading();
    sockets_.insert( std::make_pair( address, pSocket ) );
    connection_.ConnectionSucceeded( address );
}

// -----------------------------------------------------------------------------
// Name: SocketManager::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void SocketManager::Send( const std::string& endpoint, unsigned long tag, const Message& message )
{
    const CIT_Sockets it = sockets_.find( endpoint );
    if( it == sockets_.end() )
        throw std::runtime_error( "Not connected to " + endpoint );
    it->second->Send( tag, message );
}
