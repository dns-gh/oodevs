// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Server.h"
#include "Client.h"
#include "Listener_ABC.h"

using namespace shield;

// -----------------------------------------------------------------------------
// Name: Server constructor
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
Server::Server( unsigned short port, const std::string& host, Listener_ABC& listener )
    : tools::ServerNetworker( port )
    , host_    ( host )
    , listener_( listener )
{
    AllowConnections();
}

// -----------------------------------------------------------------------------
// Name: Server destructor
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
Server::~Server()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Server::Update
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
void Server::Update()
{
    tools::ServerNetworker::Update();
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        it->second->Update();
    for( CIT_Errors it = errors_.begin(); it != errors_.end(); ++it )
        clients_.erase( *it );
    errors_.clear();
}

// -----------------------------------------------------------------------------
// Name: Server::ConnectionSucceeded
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
void Server::ConnectionSucceeded( const std::string& from )
{
    clients_.erase( from );
    boost::shared_ptr< Client > client( new Client( host_, from, *this, *this, *this ) );
    clients_.insert( std::make_pair( from, client ) );
    listener_.Info( "Shield proxy received connection from " + from );
    tools::ServerNetworker::ConnectionSucceeded( from );
}

// -----------------------------------------------------------------------------
// Name: Server::ConnectionFailed
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Server::ConnectionFailed( const std::string& from, const std::string& error )
{
    listener_.Error( "Shield proxy connection from " + from + " failed : " + error );
    tools::ServerNetworker::ConnectionFailed( from, error );
}

// -----------------------------------------------------------------------------
// Name: Server::ConnectionError
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Server::ConnectionError( const std::string& from, const std::string& error )
{
    if( std::find( errors_.begin(), errors_.end(), from ) != errors_.end() )
        return;
    clients_.erase( from );
    listener_.Error( "Shield proxy connection from " + from + " aborted : " + error );
    tools::ServerNetworker::ConnectionError( from, error );
}

// -----------------------------------------------------------------------------
// Name: Server::Info
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Server::Info( const std::string& /*from*/, const std::string& message )
{
    listener_.Info( message );
}

// -----------------------------------------------------------------------------
// Name: Server::Error
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Server::Error( const std::string& from, const std::string& message )
{
    errors_.push_back( from );
    listener_.Error( message );
    tools::ServerNetworker::ConnectionError( from, message );
}
