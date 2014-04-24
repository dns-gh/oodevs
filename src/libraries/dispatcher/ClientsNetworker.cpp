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
#include "Model_ABC.h"
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ClientsNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( const Config& config, Plugin_ABC& plugin, const Services& services, const Model_ABC& model )
    : ServerNetworker( config.GetNetworkClientsParameters(), config.GetNetworkTimeout() )
    , plugin_  ( plugin )
    , services_( services )
    , model_   ( model )
{
    MT_LOG_INFO_MSG( "Starting dispatcher server on address " << config.GetNetworkClientsParameters() );
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

namespace
{
    bool ShouldUnicast( const sword::SimToClient& message )
    {
        return message.has_client_id() &&
            ( ( message.message().has_compute_pathfind_ack() && !message.message().compute_pathfind_ack().has_id() ) ||
              message.message().has_segment_request_ack() );
    }
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
    else if( message.message().has_control_begin_tick() )
        OnNewTick();
    if( ShouldUnicast( message ) )
        Unicast( message );
    else
        Broadcast( message );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Broadcast
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void ClientsNetworker::Broadcast( const sword::SimToClient& message )
{
    for( auto it = internals_.begin(); it != internals_.end(); ++it )
        it->second->Send( message );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Unicast
// Created: LGY 2014-03-06
// -----------------------------------------------------------------------------
void ClientsNetworker::Unicast( const sword::SimToClient& message )
{
    auto clientId = clientsId_.right.find( message.client_id() );
    if( clientId != clientsId_.right.end() )
    {
        auto client = internals_.find( clientId->second );
        if( client != internals_.end() )
            client->second->Send( message );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::NotifyClientAuthenticated
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void ClientsNetworker::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, const std::string& link,
                                                  dispatcher::Profile_ABC& /*profile*/, unsigned int clientId, bool uncounted )
{
    boost::shared_ptr< Client > pClient = clients_[ link ];
    internals_[ link ] = pClient;
    clientsId_.insert( T_ClientsId::value_type( link, clientId ) );
    if( uncounted )
        uncountedClients_.insert( link );
    model_.Send( *pClient );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::NotifyClientLeft
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void ClientsNetworker::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/, const std::string& link, bool /*uncounted*/ )
{
    uncountedClients_.erase( link );
    internals_.erase( link );
    clientsId_.left.erase( link );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionSucceeded
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionSucceeded( const std::string& local, const std::string& remote )
{
    MT_LOG_INFO_MSG( "Connection received from client '" << remote << "'" );
    ServerNetworker::ConnectionSucceeded( local, remote );
    boost::shared_ptr< Client >& pClient = clients_[ remote ];
    pClient.reset( new Client( *this, remote ) );
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
    ServerNetworker::ConnectionError( link, reason );
    auto it = clients_.find( link );
    if( it != clients_.end() && it->second )
    {
        MT_LOG_INFO_MSG( "Connection to '" << link << "' lost (" << reason << ")" );
        plugin_.NotifyClientLeft( *it->second, it->first, uncountedClients_.find( link ) != uncountedClients_.end() );
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
        for( auto it = clients_.begin(); it != clients_.end(); ++it ) // $$$$ MCO : doesn't this bypass authentication ?
            it->second->Send( msg );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << tools::GetExceptionMsg( e ) );
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
        // Obviously we must send authentication responses to unauthenticated
        // clients. The connected_profile_list passthrough is historical and
        // and might be removed in the future.
        const T_Clients* receivers = &internals_;
        if( msg.message().has_authentication_response() ||
                msg.message().has_connected_profile_list() )
            receivers = &clients_;
        for( auto it = receivers->begin(); it != receivers->end(); ++it )
            it->second->Send( msg );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << tools::GetExceptionMsg( e ) );
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
        for( auto it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << tools::GetExceptionMsg( e ) );
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
        for( auto it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
        plugin_.Receive( msg );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << tools::GetExceptionMsg( e ) );
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
        for( auto it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
        plugin_.Receive( msg );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << tools::GetExceptionMsg( e ) );
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
        for( auto it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::GetConnectedPublisher
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
ClientPublisher_ABC& ClientsNetworker::GetConnectedPublisher( const std::string& link ) const
{
    auto it = clients_.find( link );
    if( it == clients_.end() || !it->second )
        throw MASA_EXCEPTION( link + " is not a valid client" );
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnNewTick
// Created: LDC 2011-11-22
// -----------------------------------------------------------------------------
void ClientsNetworker::OnNewTick()
{
    std::vector< std::string > errors;
    for( auto it = clients_.begin(); it != clients_.end(); ++it )
        if( ! it->second->HasAnsweredSinceLastTick() )
            errors.push_back( it->first );
    for( auto it = errors.begin(); it != errors.end(); ++it )
        MT_LOG_ERROR_MSG( "Client hasn't answered messages from last tick! Client should be checked or disconnected: " << *it );
}
