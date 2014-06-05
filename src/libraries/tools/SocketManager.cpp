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
#include <boost/ref.hpp>
#include <boost/smart_ptr/make_shared.hpp>

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
    , outBytes_      ( 0 )
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
    const std::string local = ToString( socket->local_endpoint() );
    const std::string remote = ToString( socket->remote_endpoint() );
    boost::shared_ptr< Socket > pSocket = boost::make_shared< Socket >( socket, message_, remote );
    SOCKET nativeSock = socket->native();
    if( nativeSock !=0 )
    {
        setsockopt( nativeSock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast< const char* >( &timeOut_ ), sizeof( DWORD) );
        setsockopt( nativeSock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast< const char* >( &timeOut_ ), sizeof( DWORD) );
    }
    sockets_.insert( std::make_pair( remote, pSocket ) );
    connection_->ConnectionSucceeded( local, remote );
    pSocket->StartReading();
}

// -----------------------------------------------------------------------------
// Name: SocketManager::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void SocketManager::Send( const std::string& endpoint, unsigned long tag, Message& message )
{
    const CIT_Sockets it = sockets_.find( endpoint );
    if( it == sockets_.end() )
        throw MASA_EXCEPTION( "Not connected to " + endpoint );
    outBytes_ += it->second->Send( tag, message );
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

uint64_t SocketManager::GetSentAmount() const
{
    return outBytes_;
}
