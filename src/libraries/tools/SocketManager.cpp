// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "SocketManager.h"
#include "Socket.h"
#include "ConnectionCallback_ABC.h"
#include "MT_Tools/MT_Logger.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: SocketManager constructor
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
SocketManager::SocketManager( boost::shared_ptr< MessageCallback_ABC > message,
                              boost::shared_ptr< ConnectionCallback_ABC > connection, DWORD timeOut )
    : message_       ( message )
    , connection_    ( connection )
    , nbMessagesSent_( 0 )
    , timeOut_       ( timeOut )
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
    boost::shared_ptr< Socket > pSocket( new Socket( socket, message_, address ) );
    SOCKET nativeSock = socket->native();
    if( nativeSock !=0 )
    {
        setsockopt( nativeSock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast< const char* >( &timeOut_ ), sizeof( DWORD) );
        setsockopt( nativeSock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast< const char* >( &timeOut_ ), sizeof( DWORD) );
    }
    pSocket->StartReading();
    sockets_.insert( std::make_pair( address, pSocket ) );
    connection_->ConnectionSucceeded( address );
}

// -----------------------------------------------------------------------------
// Name: SocketManager::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void SocketManager::Send( const std::string& endpoint, unsigned long tag, Message& message )
{
    const CIT_Sockets it = sockets_.find( endpoint );
    if( it == sockets_.end() )
        throw std::runtime_error( "Not connected to " + endpoint );
    int nMessagesQueued = it->second->Send( tag, message );
    // MT_LOG_INFO_MSG( "Queued " << nMessagesQueued << " messages for " << endpoint );
    ++nbMessagesSent_;
}
