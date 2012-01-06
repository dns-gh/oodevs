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
#include "Logger.h"
#include "Listener_ABC.h"
#pragma warning( push, 0 )
#include "shield/proto/AarToClient.pb.h"
#include "shield/proto/AuthenticationToClient.pb.h"
#include "shield/proto/DispatcherToClient.pb.h"
#include "shield/proto/MessengerToClient.pb.h"
#include "shield/proto/ReplayToClient.pb.h"
#include "shield/proto/SimToClient.pb.h"
#include "shield/proto/LauncherToAdmin.pb.h"
#include "shield/proto/ClientToAar.pb.h"
#include "shield/proto/ClientToAuthentication.pb.h"
#include "shield/proto/ClientToMessenger.pb.h"
#include "shield/proto/ClientToReplay.pb.h"
#include "shield/proto/ClientToSim.pb.h"
#include "shield/proto/AdminToLauncher.pb.h"
#pragma warning( pop )
#include <boost/lexical_cast.hpp>

#pragma warning( disable : 4355 )

using namespace shield;

namespace
{
    template< typename C, typename T >
    boost::function< void( const std::string&, const T& ) > MakeLogger(
        ClientListener_ABC& listener, C& instance,
        void (C::*callback)( const std::string&, const T& ) )
    {
        return Logger< T >( listener, boost::bind( callback, &instance, _1, _2 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Server constructor
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
Server::Server( unsigned short port, tools::MessageDispatcher_ABC& dispatcher,
                const Model_ABC& model, ClientHandler_ABC& handler, Listener_ABC& listener,
                bool encodeStringsInUtf8, unsigned long timeOut /*= 0*/ )
    : tools::ServerNetworker( port, timeOut )
    , listener_( listener )
    , clients_ ( *this, dispatcher, model, handler, *this, encodeStringsInUtf8 )
{
    listener_.Info( "Starting shield server on port " + boost::lexical_cast< std::string >( port ) );
    RegisterMessage( MakeLogger( *this, *this, &Server::ReceiveClientToAar ) );
    RegisterMessage( MakeLogger( *this, *this, &Server::ReceiveClientToAuthentication ) );
    RegisterMessage( MakeLogger( *this, *this, &Server::ReceiveClientToMessenger ) );
    RegisterMessage( MakeLogger( *this, *this, &Server::ReceiveClientToReplay ) );
    RegisterMessage( MakeLogger( *this, *this, &Server::ReceiveClientToSim ) );
    RegisterMessage( MakeLogger( *this, *this, &Server::ReceiveAdminToLauncher ) );
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
void Server::Update( bool allowConnections )
{
	if ( allowConnections )
		tools::ServerNetworker::AllowConnections();
	else
		tools::ServerNetworker::DenyConnections();
    tools::ServerNetworker::Update();
    for( CIT_Errors it = errors_.begin(); it != errors_.end(); ++it )
        clients_.Remove( *it );
    errors_.clear();
}

// -----------------------------------------------------------------------------
// Name: Server::ConnectionSucceeded
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
void Server::ConnectionSucceeded( const std::string& from )
{
    clients_.Add( from );
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
    clients_.Remove( from );
    listener_.Error( "Shield proxy connection from " + from + " aborted : " + error );
    tools::ServerNetworker::ConnectionError( from, error );
}

// -----------------------------------------------------------------------------
// Name: Server::ConnectionWarning
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Server::ConnectionWarning( const std::string& from, const std::string& warning )
{
    listener_.Error( "Shield proxy connection from " + from + " warning : " + warning );
    tools::ServerNetworker::ConnectionWarning( from, warning );
}

// -----------------------------------------------------------------------------
// Name: Server::Warning
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Server::Warning( const std::string& from, const std::string& warning )
{
    listener_.Error( "Shield proxy connection from " + from + " warning : " + warning );
}

// -----------------------------------------------------------------------------
// Name: Server::Info
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Server::Info( const std::string& message )
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

// -----------------------------------------------------------------------------
// Name: Server::Debug
// Created: MCO 2011-06-23
// -----------------------------------------------------------------------------
void Server::Debug( const DebugInfo_ABC& info )
{
    listener_.Debug( info );
}

// -----------------------------------------------------------------------------
// Name: Server::ReceiveClientToAar
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Server::ReceiveClientToAar( const std::string& from, const MsgsClientToAar::MsgClientToAar& msg )
{
    clients_.ReceiveClientToAar( from, msg );
}

// -----------------------------------------------------------------------------
// Name: Server::ReceiveClientToAuthentication
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Server::ReceiveClientToAuthentication( const std::string& from, const MsgsClientToAuthentication::MsgClientToAuthentication& msg )
{
    clients_.ReceiveClientToAuthentication( from, msg );
}

// -----------------------------------------------------------------------------
// Name: Server::ReceiveClientToMessenger
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Server::ReceiveClientToMessenger( const std::string& from, const MsgsClientToMessenger::MsgClientToMessenger& msg )
{
    clients_.ReceiveClientToMessenger( from, msg );
}

// -----------------------------------------------------------------------------
// Name: Server::ReceiveClientToReplay
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Server::ReceiveClientToReplay( const std::string& from, const MsgsClientToReplay::MsgClientToReplay& msg )
{
    clients_.ReceiveClientToReplay( from, msg );
}

// -----------------------------------------------------------------------------
// Name: Server::ReceiveClientToSim
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Server::ReceiveClientToSim( const std::string& from, const MsgsClientToSim::MsgClientToSim& msg )
{
    clients_.ReceiveClientToSim( from, msg );
}

// -----------------------------------------------------------------------------
// Name: Server::ReceiveAdminToLauncher
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Server::ReceiveAdminToLauncher( const std::string& from, const MsgsAdminToLauncher::MsgAdminToLauncher& msg )
{
    clients_.ReceiveAdminToLauncher( from, msg );
}
