// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include <stdexcept>
#include <iostream>
#include "tools/ClientNetworker.h"
#pragma warning( push, 0 )
#include "shield/proto/ClientToAar.pb.h"
#include "shield/proto/ClientToAuthentication.pb.h"
#include "shield/proto/ClientToMessenger.pb.h"
#include "shield/proto/ClientToReplay.pb.h"
#include "shield/proto/ClientToSim.pb.h"
#include "shield/proto/AdminToLauncher.pb.h"
#include "shield/proto/AarToClient.pb.h"
#include "shield/proto/MessengerToClient.pb.h"
#include "shield/proto/DispatcherToClient.pb.h"
#include "shield/proto/ReplayToClient.pb.h"
#include "shield/proto/SimToClient.pb.h"
#include "shield/proto/LauncherToAdmin.pb.h"
#pragma warning( pop )

#pragma warning( disable: 4127 )

namespace
{
    class Client : public tools::ClientNetworker
    {
    public:
        explicit Client( const std::string& host )
            : tools::ClientNetworker( host )
        {
            RegisterMessage( *this, &Client::ReceiveSimToClient );
            RegisterMessage( *this, &Client::ReceiveAuthenticationToClient );
            RegisterMessage( *this, &Client::ReceiveDispatcherToClient );
            RegisterMessage( *this, &Client::ReceiveMessengerToClient );
            RegisterMessage( *this, &Client::ReceiveReplayToClient );
            RegisterMessage( *this, &Client::ReceiveAarToClient );
        }
        void Send( const MsgsClientToAar::MsgClientToAar& message )
        {
            tools::MessageSender_ABC::Send( host_, message );
        }
        void Send( const MsgsClientToAuthentication::MsgClientToAuthentication& message )
        {
            tools::MessageSender_ABC::Send( host_, message );
        }
        void Send( const MsgsClientToMessenger::MsgClientToMessenger& message )
        {
            tools::MessageSender_ABC::Send( host_, message );
        }
        void Send( const MsgsClientToReplay::MsgClientToReplay& message )
        {
            tools::MessageSender_ABC::Send( host_, message );
        }
        void Send( const MsgsClientToSim::MsgClientToSim& message )
        {
            tools::MessageSender_ABC::Send( host_, message );
        }
        void Send( const MsgsAdminToLauncher::MsgAdminToLauncher& message )
        {
            tools::MessageSender_ABC::Send( host_, message );
        }
    private:
        void ReceiveSimToClient( const std::string& link, const MsgsSimToClient::MsgSimToClient& message )
        {
            std::cout << "received from " << link << " : " << message.ShortDebugString() << std::endl;
        }
        void ReceiveAuthenticationToClient( const std::string& link, const MsgsAuthenticationToClient::MsgAuthenticationToClient& message )
        {
            std::cout << "received from " << link << " : " << message.ShortDebugString() << std::endl;
        }
        void ReceiveReplayToClient( const std::string& link, const MsgsReplayToClient::MsgReplayToClient& message )
        {
            std::cout << "received from " << link << " : " << message.ShortDebugString() << std::endl;
        }
        void ReceiveAarToClient( const std::string& link, const MsgsAarToClient::MsgAarToClient& message )
        {
            std::cout << "received from " << link << " : " << message.ShortDebugString() << std::endl;
        }
        void ReceiveMessengerToClient( const std::string& link, const MsgsMessengerToClient::MsgMessengerToClient& message )
        {
            std::cout << "received from " << link << " : " << message.ShortDebugString() << std::endl;
        }
        void ReceiveDispatcherToClient( const std::string& link, const MsgsDispatcherToClient::MsgDispatcherToClient& message )
        {
            std::cout << "received from " << link << " : " << message.ShortDebugString() << std::endl;
        }
    private:
        virtual void ConnectionSucceeded( const std::string& host )
        {
            host_ = host;
            std::cout << "connection succeeded : " << host << std::endl;
            tools::ClientNetworker::ConnectionSucceeded( host );
            MsgsClientToAuthentication::MsgClientToAuthentication request;
            request.mutable_message()->mutable_authentication_request()->set_login( "" );
            request.mutable_message()->mutable_authentication_request()->set_password( "" );
            request.mutable_message()->mutable_authentication_request()->mutable_version()->set_value( Version::ProtocolVersion().value() );
            Send( request );
        }
        virtual void ConnectionFailed( const std::string& host, const std::string& error )
        {
            std::cout << "connection failed : " << host << " : " << error << std::endl;
            tools::ClientNetworker::ConnectionFailed( host, error );
        }
        virtual void ConnectionError( const std::string& host, const std::string& error )
        {
            std::cout << "connection error : " << host << " : " << error << std::endl;
            tools::ClientNetworker::ConnectionError( host, error );
        }
    private:
        std::string host_;
    };
}

int main( int /*argc*/, char* /*argv*/[] )
{
    try
    {
        Client client( "localhost:30001" );
        while( true )
            client.Update();
    }
    catch( std::exception& e )
    {
        std::cout << e.what() << std::endl;
    }
    catch( ... )
    {
        std::cout << "unknown error" << std::endl;
    }
    return 0;
}
