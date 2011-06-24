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
#include "DebugInfo_ABC.h"
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

namespace
{
    template< typename T >
    class DebugInfo : public DebugInfo_ABC
    {
    public:
        DebugInfo( const char* prefix, const T& message )
            : prefix_  ( prefix )
            , pMessage_( &message )
        {}
        virtual void Serialize( std::ostream& s ) const
        {
            s << prefix_ << pMessage_->ShortDebugString();
        }
    private:
        const char* prefix_;
        const T* pMessage_;
    };

    template< typename T >
    class Logger
    {
    private:
        typedef boost::function< void( const std::string&, const T& ) > T_Callback;

    public:
        Logger( ClientListener_ABC& listener, T_Callback callback )
            : pListener_( &listener )
            , callback_ ( callback )
        {}

        void operator()( const std::string& link, const T& message ) const
        {
            callback_( link, message );
            pListener_->Debug( DebugInfo< T >( "Shield received : ", message ) );
        }

    private:
        ClientListener_ABC* pListener_;
        T_Callback callback_;
    };

    template< typename C, typename T >
    boost::function< void( const std::string&, const T& ) > MakeLogger( ClientListener_ABC& listener, C& instance, void (C::*callback)( const std::string&, const T& ) )
    {
        return Logger< T >( listener, boost::bind( callback, &instance, _1, _2 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Client::Client( const std::string& host, const std::string& from, tools::MessageSender_ABC& sender,
                tools::MessageDispatcher_ABC& dispatcher, ClientListener_ABC& listener )
    : tools::ClientNetworker( host )
    , from_     ( from )
    , sender_   ( sender )
    , listener_ ( listener )
    , converter_( from, *this, *this, listener )
{
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveSimToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveAuthenticationToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveDispatcherToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveMessengerToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveReplayToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveAarToClient ) );
    RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveLauncherToAdmin ) );
    dispatcher.RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveClientToAar ) );
    dispatcher.RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveClientToAuthentication ) );
    dispatcher.RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveClientToMessenger ) );
    dispatcher.RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveClientToReplay ) );
    dispatcher.RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveClientToSim ) );
    dispatcher.RegisterMessage( MakeLogger( listener, converter_, &Converter::ReceiveAdminToLauncher ) );
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
// Name: Client::ConnectionSucceeded
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Client::ConnectionSucceeded( const std::string& host )
{
    host_ = host;
    tools::ClientNetworker::ConnectionSucceeded( host );
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
    tools::ClientNetworker::ConnectionFailed( host, error );
    listener_.Error( from_, "Shield proxy connection to " + host + " failed : " + error );
}

// -----------------------------------------------------------------------------
// Name: Client::ConnectionError
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Client::ConnectionError( const std::string& host, const std::string& error )
{
    tools::ClientNetworker::ConnectionError( host, error );
    listener_.Error( from_, "Shield proxy connection to " + host + " aborted : " + error );
}

// -----------------------------------------------------------------------------
// Name: Client::DoSend
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
void Client::DoSend( const T& message )
{
    if( host_.empty() )
        callbacks_.push_back( boost::bind( &tools::MessageSender_ABC::Send< T >, this, boost::cref( host_ ), message ) );
    else
        tools::MessageSender_ABC::Send( host_, message );
    listener_.Debug( DebugInfo< T >( "Shield sent : ", message ) );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToSim& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToAuthentication& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToReplay& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToAar& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToMessenger& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Client::Send( const sword::AdminToLauncher& message )
{
    DoSend( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const MsgsSimToClient::MsgSimToClient& message )
{
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const MsgsAuthenticationToClient::MsgAuthenticationToClient& message )
{
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const MsgsReplayToClient::MsgReplayToClient& message )
{
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const MsgsAarToClient::MsgAarToClient& message )
{
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const MsgsMessengerToClient::MsgMessengerToClient& message )
{
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-10-27
// -----------------------------------------------------------------------------
void Client::Send( const MsgsDispatcherToClient::MsgDispatcherToClient& message )
{
    sender_.Send( from_, message );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Client::Send( const MsgsLauncherToAdmin::MsgLauncherToAdmin& message )
{
    sender_.Send( from_, message );
}
