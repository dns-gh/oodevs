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
#include "Utf8Converter.h"
#include "DebugInfo.h"
#include "tools/MessageSender_ABC.h"
#include "tools/MessageDispatcher_ABC.h"
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

#pragma warning( disable: 4355 )

using namespace shield;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Client::Client( const std::string& from, tools::MessageSender_ABC& sender, tools::MessageDispatcher_ABC& dispatcher,
                ClientListener_ABC& listener, bool encodeStringsInUtf8 )
    : from_               ( from )
    , sender_             ( sender )
    , dispatcher_         ( dispatcher )
    , listener_           ( listener )
    , converter_          ( *this, *this, listener )
    , encodeStringsInUtf8_( encodeStringsInUtf8 )
{
    service_.RegisterMessage< sword::AarToClient >( boost::bind( &Converter::ReceiveAarToClient, &converter_, _2 ) );
    service_.RegisterMessage< sword::AuthenticationToClient >( boost::bind( &Converter::ReceiveAuthenticationToClient, &converter_, _2 ) );
    service_.RegisterMessage< sword::DispatcherToClient >( boost::bind( &Converter::ReceiveDispatcherToClient, &converter_, _2 ) );
    service_.RegisterMessage< sword::MessengerToClient >( boost::bind( &Converter::ReceiveMessengerToClient, &converter_, _2 ) );
    service_.RegisterMessage< sword::ReplayToClient >( boost::bind( &Converter::ReceiveReplayToClient, &converter_, _2 ) );
    service_.RegisterMessage< sword::SimToClient >( boost::bind( &Converter::ReceiveSimToClient, &converter_, _2 ) );
    service_.RegisterMessage< sword::LauncherToAdmin >( boost::bind( &Converter::ReceiveLauncherToAdmin, &converter_, _2 ) );
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
// Name: Client::ReceiveClientToAar
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToAar( const MsgsClientToAar::MsgClientToAar& message )
{
    converter_.ReceiveClientToAar( message );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveClientToAuthentication
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToAuthentication( const MsgsClientToAuthentication::MsgClientToAuthentication& message )
{
    converter_.ReceiveClientToAuthentication( message );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveClientToMessenger
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToMessenger( const MsgsClientToMessenger::MsgClientToMessenger& message )
{
    converter_.ReceiveClientToMessenger( message );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveClientToReplay
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToReplay( const MsgsClientToReplay::MsgClientToReplay& message )
{
    converter_.ReceiveClientToReplay( message );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveClientToSim
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveClientToSim( const MsgsClientToSim::MsgClientToSim& message )
{
    converter_.ReceiveClientToSim( message );
}

// -----------------------------------------------------------------------------
// Name: Client::ReceiveAdminToLauncher
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Client::ReceiveAdminToLauncher( const MsgsAdminToLauncher::MsgAdminToLauncher& message )
{
    converter_.ReceiveAdminToLauncher( message );
}

// -----------------------------------------------------------------------------
// Name: Client::DoSend
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
void Client::DoSend( T& message )
{
    if( encodeStringsInUtf8_ )
        Utf8Converter::ConvertUtf8StringsToCP1252( message );
    dispatcher_.Receive( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2011-11-03
// -----------------------------------------------------------------------------
void Client::Send( const std::string& link, unsigned long tag, const google::protobuf::Message& m )
{
    service_.Receive( link, tag, m );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2011-11-03
// -----------------------------------------------------------------------------
void Client::Send( const std::string&, unsigned long, tools::Message& )
{
    throw std::runtime_error( __FUNCTION__ ); // $$$$ MCO : pretty crappy but can actually never happen...
}

// -----------------------------------------------------------------------------
// Name: Client::GetNbMessagesSent
// Created: MCO 2011-11-03
// -----------------------------------------------------------------------------
unsigned long Client::GetNbMessagesSent() const
{
    return 0;
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
// Name: Client::Notify
// Created: MCO 2011-10-12
// -----------------------------------------------------------------------------
template< typename T >
void Client::Notify( const T& message ) const
{
    listener_.Debug( DebugInfo< T >( "Shield sent : ", message ) );
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
    Notify( message );
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
    Notify( message );
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
    Notify( message );
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
    Notify( message );
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
    Notify( message );
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
    Notify( message );
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
    Notify( message );
}
