// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ClientsNetworker.h"
#include "Client.h"
#include "Config.h"
#include "Plugin_ABC.h"
#include "Services.h"
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ClientsNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( const Config& config, Plugin_ABC& plugin, const Services& services )
    : ServerNetworker( config.GetNetworkClientsParameters(), config.GetNetworkTimeout() )
    , plugin_  ( plugin )
    , services_( services )
{
    MT_LOG_INFO_MSG( "Starting dispatcher server on port " << config.GetNetworkClientsParameters() );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::~ClientsNetworker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Update
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void ClientsNetworker::Update()
{
    ServerNetworker::Update();
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Receive
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void ClientsNetworker::Receive( const sword::SimToClient& message )
{
    if( message.message().has_control_send_current_state_begin() )
        DenyConnections();
    else if( message.message().has_control_send_current_state_end() )
        AllowConnections();
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Activate
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void ClientsNetworker::Activate( const std::string& link )
{
    internals_[ link ] = clients_[ link ];
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Deactivate
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void ClientsNetworker::Deactivate( const std::string& link )
{
    internals_.erase( link );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Broadcast
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void ClientsNetworker::Broadcast( const sword::SimToClient& message )
{
    static const unsigned long tag = tools::MessageIdentifierFactory::GetIdentifier< sword::SimToClient >();
    const tools::Message m = Serialize( message );
    for( CIT_Clients it = internals_.begin(); it != internals_.end(); ++it )
        it->second->Send( tag, m );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::NotifyClientAuthenticated
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void ClientsNetworker::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, const std::string& link, dispatcher::Profile_ABC& /*profile*/ )
{
    CIT_Clients it = clients_.find( link );
    if( it != clients_.end() )
        it->second->Activate();
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::NotifyClientLeft
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void ClientsNetworker::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/, const std::string& link )
{
    CIT_Clients it = clients_.find( link );
    if( it != clients_.end() )
        it->second->Deactivate();
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Register
// Created: MCO 2011-10-28
// -----------------------------------------------------------------------------
void ClientsNetworker::Register( const std::string& link, MessageSender_ABC& sender, ClientBroadcaster_ABC& broadcaster )
{
    MT_LOG_INFO_MSG( "Publisher registered for client '" << link << "'" );
    boost::shared_ptr< Client >& pClient = clients_[ link ];
    pClient.reset( new Client( sender, broadcaster, link ) );
    services_.Send( *pClient );
    MT_LOG_INFO_MSG( clients_.size() << " clients connected" );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Unregister
// Created: MCO 2011-10-28
// -----------------------------------------------------------------------------
void ClientsNetworker::Unregister( const std::string& link )
{
    MT_LOG_INFO_MSG( "Publisher unregistered for client '" << link << "'" );
    IT_Clients it = clients_.find( link );
    if( it != clients_.end() )
    {
        plugin_.NotifyClientLeft( *it->second, it->first );
        clients_.erase( it );
    }
    MT_LOG_INFO_MSG( clients_.size() << " clients connected" );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionSucceeded
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionSucceeded( const std::string& link )
{
    MT_LOG_INFO_MSG( "Connection received from client '" << link << "'" );
    ServerNetworker::ConnectionSucceeded( link );
    boost::shared_ptr< Client >& pClient = clients_[ link ];
    pClient.reset( new Client( *this, *this, link ) );
    services_.Send( *pClient );
    MT_LOG_INFO_MSG( clients_.size() << " clients connected" );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionFailed
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionFailed( const std::string& address, const std::string& error )
{
    MT_LOG_INFO_MSG( "Bad connection received from client '" << address << "' (" << error << ")" );
    ServerNetworker::ConnectionFailed( address, error );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionError
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionError( const std::string& link, const std::string& reason )
{
    MT_LOG_INFO_MSG( "Connection to '" << link << "' lost (" << reason << ")" );
    ServerNetworker::ConnectionError( link, reason );
    IT_Clients it = clients_.find( link );
    if( it != clients_.end() && it->second )
    {
        plugin_.NotifyClientLeft( *it->second, it->first );
        clients_.erase( it );
    }
    MT_LOG_INFO_MSG( clients_.size() << " clients connected" );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionWarning
// Created: MCO 2011-09-26
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionWarning( const std::string& link, const std::string& warning )
{
    MT_LOG_INFO_MSG( "Connection to '" << link << "' warning (" << warning << ")" );
    ServerNetworker::ConnectionWarning( link, warning );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const sword::SimToClient& msg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it ) // $$$$ MCO : doesn't this bypass authentication ?
            it->second->Send( msg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const sword::AuthenticationToClient& msg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const sword::ReplayToClient& msg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const sword::AarToClient& msg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
        plugin_.Receive( msg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const sword::MessengerToClient& msg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
        plugin_.Receive( msg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const sword::DispatcherToClient& msg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::GetProfile
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
Profile_ABC& ClientsNetworker::GetProfile( const std::string& )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::GetPublisher
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
ClientPublisher_ABC& ClientsNetworker::GetPublisher( const std::string& link )
{
    CIT_Clients it = clients_.find( link );
    if( it == clients_.end() || !it->second )
        throw std::runtime_error( link + " is not a valid client" );
    return *it->second;
}
