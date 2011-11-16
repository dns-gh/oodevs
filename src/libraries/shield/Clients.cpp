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
#include "ClientListener_ABC.h"
#include "Utf8Converter.h"
#include "DebugInfo.h"
#pragma warning( push, 0 )
#include "proto/SimToClient.pb.h"
#pragma warning( pop )
#include <boost/preprocessor/stringize.hpp>

using namespace shield;

// -----------------------------------------------------------------------------
// Name: Clients constructor
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
Clients::Clients( tools::MessageSender_ABC& sender, tools::MessageDispatcher_ABC& dispatcher,
                  const Model_ABC& model, ClientHandler_ABC& handler, ClientListener_ABC& listener, bool encodeStringsInUtf8 )
    : model_             ( model )
    , sender_            ( sender )
    , dispatcher_        ( dispatcher )
    , handler_           ( handler )
    , listener_          ( listener )
    , cache_             ( sender, model, encodeStringsInUtf8 )
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
    boost::shared_ptr< Client > client( new Client( from, sender_, dispatcher_, listener_, utf8StringEncoding_ ) );
    clients_.insert( std::make_pair( from, client ) );
    handler_.Register( from, *client, *this );
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

// -----------------------------------------------------------------------------
// Name: Clients::Activate
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void Clients::Activate( const std::string& link )
{
    actives_[ link ] = clients_[ link ];
    cache_.Send( link );
}

// -----------------------------------------------------------------------------
// Name: Clients::Deactivate
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void Clients::Deactivate( const std::string& link )
{
    actives_.erase( link );
}

// -----------------------------------------------------------------------------
// Name: Clients::Broadcast
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void Clients::Broadcast( const sword::SimToClient& message )
{
    cache_.Clear();
    if( ! actives_.empty() )
    {
        Converter converter( *this, *this );
        converter.ReceiveSimToClient( message );
    }
}

#define NO_BROADCAST( MESSAGE ) \
    void Clients::Send( MESSAGE& ) \
    { \
        throw std::runtime_error( "unable to broadcast message of type " BOOST_PP_STRINGIZE( MESSAGE ) ); \
    } \

NO_BROADCAST( sword::ClientToSim )
NO_BROADCAST( sword::ClientToAuthentication )
NO_BROADCAST( sword::ClientToReplay )
NO_BROADCAST( sword::ClientToAar )
NO_BROADCAST( sword::ClientToMessenger )
NO_BROADCAST( sword::AdminToLauncher )
NO_BROADCAST( MsgsAarToClient::MsgAarToClient )
NO_BROADCAST( MsgsAuthenticationToClient::MsgAuthenticationToClient )
NO_BROADCAST( MsgsDispatcherToClient::MsgDispatcherToClient )
NO_BROADCAST( MsgsMessengerToClient::MsgMessengerToClient )
NO_BROADCAST( MsgsReplayToClient::MsgReplayToClient )
NO_BROADCAST( MsgsLauncherToAdmin::MsgLauncherToAdmin )

// -----------------------------------------------------------------------------
// Name: Clients::Send
// Created: MCO 2011-11-07
// -----------------------------------------------------------------------------
void Clients::Send( MsgsSimToClient::MsgSimToClient& message )
{
    if( utf8StringEncoding_ )
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
    static const unsigned long tag = tools::MessageIdentifierFactory::GetIdentifier< MsgsSimToClient::MsgSimToClient >();
    tools::Message m;
    sender_.Serialize( message, m );
    for( CIT_Clients it = actives_.begin(); it != actives_.end(); ++it )
    {
        sender_.Send( it->first, tag, m );
        listener_.Debug( DebugInfo< MsgsSimToClient::MsgSimToClient >( "Shield sent : ", message ) );
    }
}
