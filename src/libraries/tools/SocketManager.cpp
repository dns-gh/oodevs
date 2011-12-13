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
#include "SocketEventCallback_ABC.h"
#include "MT_Tools/MT_Logger.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: SocketManager constructor
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
SocketManager::SocketManager( boost::shared_ptr< SocketEventCallback_ABC > callback, DWORD timeOut )
    : callback_      ( callback)
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
    boost::shared_ptr< Socket > pSocket( new Socket( socket, callback_, address ) );
    SOCKET nativeSock = socket->native();
    if( nativeSock !=0 )
    {
        setsockopt( nativeSock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast< const char* >( &timeOut_ ), sizeof( DWORD) ); //$$ USELESS, to be replaced by SO_KEEPALIVE
        setsockopt( nativeSock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast< const char* >( &timeOut_ ), sizeof( DWORD) );
    }
    pSocket->StartReading();
    sockets_.insert( std::make_pair( address, pSocket ) );
    callback_->ConnectionSucceeded( address ); //$$$ NLD - 20111213 - Why is it done after "StartReading()" ?
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
    it->second->Send( tag, message );
    ++nbMessagesSent_;
}

// -----------------------------------------------------------------------------
// Name: SocketManager::HasAnsweredSinceLastTick
// Created: LDC 2011-11-23
// -----------------------------------------------------------------------------
bool SocketManager::HasAnsweredSinceLastTick( const std::string& endpoint )
{
    const CIT_Sockets it = sockets_.find( endpoint );
    if( it == sockets_.end() )
        return false;
    return it->second->HasAnsweredSinceLastTick();
}