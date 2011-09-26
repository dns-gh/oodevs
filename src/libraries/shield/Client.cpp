// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Client.h"
#include "ClientListener_ABC.h"
#include "Logger.h"
#include "Utf8Converter.h"
#pragma warning( push, 0 )
#include "proto/ClientToAar.pb.h"
#include "proto/ClientToAuthentication.pb.h"
#include "proto/ClientToMessenger.pb.h"
#include "proto/ClientToReplay.pb.h"
#include "proto/ClientToSim.pb.h"
#include "proto/AdminToLauncher.pb.h"
#include "proto/AarToClient.pb.h"
#include "proto/MessengerToClient.pb.h"
#include "proto/DispatcherToClient.pb.h"
#include "proto/ReplayToClient.pb.h"
#include "proto/SimToClient.pb.h"
#include "proto/LauncherToAdmin.pb.h"
#pragma warning( pop )
#include "tools/SocketManager.h"
#include "tools/Connector.h"
#include "tools/BufferedMessageCallback.h"
#include "tools/BufferedConnectionCallback.h"
#include "tools/ObjectMessageService.h"

#pragma warning( disable: 4355 )

using namespace shield;

namespace
{
    template< typename C, typename T >
    boost::function< void( const std::string&, const T& ) > MakeLogger(
        ClientListener_ABC& listener,
        C& instance, void (C::*callback)( const T& ) )
    {
        return Logger< T >( listener, boost::bind( callback, &instance, _2 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Client::Client( boost::asio::io_service& service, const std::string& from,
                tools::MessageSender_ABC& sender, ClientListener_ABC& listener, bool encodeStringsInUtf8 )
    : from_               ( from )
    , sender_             ( sender )
    , listener_           ( listener )
    , converter_          ( *this, *this, listener )
    , encodeStringsInUtf8_( encodeStringsInUtf8 )
    , connectionBuffer_   ( new tools::BufferedConnectionCallback() )
    , messageBuffer_      ( new tools::BufferedMessageCallback() )
    , sockets_            ( new tools::SocketManager( messageBuffer_, connectionBuffer_ ) )
    , messageService_     ( new tools::ObjectMessageService() )
    , connector_          ( new tools::Connector( service, *sockets_, *connectionBuffer_ ) )
{
    messageService_->RegisterErrorCallback( boost::bind( &Client::ConnectionError, this, _1, _2 ) );
    messageService_->RegisterWarningCallback( boost::bind( &Client::ConnectionWarning, this, _1, _2 ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveSimToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveAuthenticationToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveDispatcherToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveMessengerToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveReplayToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveAarToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveLauncherToAdmin ) );
}

// -----------------------------------------------------------------------------
// Name: Client destructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Client::~Client()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client::Connect
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
void Client::Connect( const std::string& host )
{
    connector_->Connect( host );
}

// -----------------------------------------------------------------------------
// Name: Client::Disconnect
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
void Client::Disconnect()
{
    sockets_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: Client::Update
// Created: NLD 2007-01-24
// -----------------------------------------------------------------------------
void Client::Update()
{
    connectionBuffer_->Commit( *this );
    messageBuffer_->Commit( *messageService_ );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Client::Send( const std::string& endpoint, unsigned long tag, const tools::Message& message )
{
    sockets_->Send( endpoint, tag, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Register
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Client::Register( unsigned long id, std::auto_ptr< tools::ObjectMessageCallback_ABC > callback )
{
    messageService_->Register( id, callback );
}

// -----------------------------------------------------------------------------
// Name: Client::Retrieve
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
tools::ObjectMessageCallback_ABC* Client::Retrieve( unsigned long id )
{
    return messageService_->Retrieve( id );
}

// -----------------------------------------------------------------------------
// Name: Client::GetNbMessagesReceived
// Created: NLD 2010-10-20
// -----------------------------------------------------------------------------
unsigned long Client::GetNbMessagesReceived() const
{
    return messageService_->GetNbMessagesReceived();
}

// -----------------------------------------------------------------------------
// Name: Client::GetNbMessagesSent
// Created: NLD 2010-10-20
// -----------------------------------------------------------------------------
unsigned long Client::GetNbMessagesSent() const
{
    return sockets_->GetNbMessagesSent();
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveClientToAar
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToAar( const MsgsClientToAar::MsgClientToAar& msg )
{
    converter_.ReceiveClientToAar( msg );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveClientToAuthentication
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToAuthentication( const MsgsClientToAuthentication::MsgClientToAuthentication& msg )
{
    converter_.ReceiveClientToAuthentication( msg );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveClientToMessenger
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToMessenger( const MsgsClientToMessenger::MsgClientToMessenger& msg )
{
    converter_.ReceiveClientToMessenger( msg );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveClientToReplay
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToReplay( const MsgsClientToReplay::MsgClientToReplay& msg )
{
    converter_.ReceiveClientToReplay( msg );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveClientToSim
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToSim( const MsgsClientToSim::MsgClientToSim& msg )
{
    converter_.ReceiveClientToSim( msg );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveAdminToLauncher
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveAdminToLauncher( const MsgsAdminToLauncher::MsgAdminToLauncher& msg )
{
    converter_.ReceiveAdminToLauncher( msg );
}

// -----------------------------------------------------------------------------
// Name: Client::ConnectionSucceeded
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Client::ConnectionSucceeded( const std::string& host )
{
    host_ = host;
    listener_.Info( "Shield proxy connected to " + host );
    std::for_each( callbacks_.begin(), callbacks_.end(), boost::apply< void >() );
    callbacks_.clear();
}

// -----------------------------------------------------------------------------
// Name: Client::ConnectionFailed
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Client::ConnectionFailed( const std::string& host, const std::string& error )
{
    listener_.Error( from_, "Shield proxy connection to " + host + " failed : " + error );
}

// -----------------------------------------------------------------------------
// Name: Client::ConnectionError
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Client::ConnectionError( const std::string& host, const std::string& error )
{
    listener_.Error( from_, "Shield proxy connection to " + host + " aborted : " + error );
}

// -----------------------------------------------------------------------------
// Name: Client::ConnectionWarning
// Created: MCO 2011-09-26
// -----------------------------------------------------------------------------
void Client::ConnectionWarning( const std::string& host, const std::string& warning )
{
    listener_.Warning( from_, "Shield proxy connection to " + host + " warning : " + warning );
}

// -----------------------------------------------------------------------------
// Name: Client::DoSend
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
void Client::DoSend( T& message )
{
    listener_.Debug( DebugInfo< T >( "Shield sent : ", message ) );
    if( encodeStringsInUtf8_ )
        Utf8Converter::ConvertUtf8StringsToCP1252( message );
    if( host_.empty() )
        callbacks_.push_back( boost::bind( &tools::MessageSender_ABC::Send< T >, this, boost::cref( host_ ), message ) );
    else
        tools::MessageSender_ABC::Send( host_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( sword::ClientToSim& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( sword::ClientToAuthentication& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( sword::ClientToReplay& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( sword::ClientToAar& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( sword::ClientToMessenger& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Client::Send( sword::AdminToLauncher& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( MsgsSimToClient::MsgSimToClient& message )
{
    if( encodeStringsInUtf8_ )
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( MsgsAuthenticationToClient::MsgAuthenticationToClient& message )
{
    if( encodeStringsInUtf8_ )
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( MsgsReplayToClient::MsgReplayToClient& message )
{
    if( encodeStringsInUtf8_ )
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( MsgsAarToClient::MsgAarToClient& message )
{
    if( encodeStringsInUtf8_ )
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( MsgsMessengerToClient::MsgMessengerToClient& message )
{
    if( encodeStringsInUtf8_ )
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( MsgsDispatcherToClient::MsgDispatcherToClient& message )
{
    if( encodeStringsInUtf8_ )
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Client::Send( MsgsLauncherToAdmin::MsgLauncherToAdmin& message )
{
    if( encodeStringsInUtf8_ )
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
    sender_.Send( from_, message );
}
