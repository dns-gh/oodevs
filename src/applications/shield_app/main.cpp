// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

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
#include <stdexcept>
#include <iostream>

#pragma warning( disable: 4127 )

namespace
{
    class Client : public tools::ClientNetworker
    {
    public:
        explicit Client( const std::string& host, const std::string& profile, const std::string& password )
            : tools::ClientNetworker( host )
            , profile_ ( profile )
            , password_( password )
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
            request.mutable_message()->mutable_authentication_request()->set_login( profile_ );
            request.mutable_message()->mutable_authentication_request()->set_password( password_ );
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
        virtual void ConnectionWarning( const std::string& host, const std::string& warning )
        {
            std::cout << "connection warning : " << host << " : " << warning << std::endl;
            tools::ClientNetworker::ConnectionWarning( host, warning );
        }
    private:
    std::string host_;
    std::string profile_;
    std::string password_;
    };
}

int main( int argc, char* argv[] )
{
    std::string host = "localhost";
    std::string port = "30001";
    std::string profile = "Supervisor";
    std::string password = "";
    if( argc == 2 && std::string( argv[1] ) == "--help" )
    {
        std::cout << "Usage: shield_app.exe [-h host] [-p port] [-u profile] [-a password]" << std::endl;
        return EXIT_SUCCESS;
    }
    for( int i = 1; i < argc - 1; ++i )
    {
        const std::string argument( argv[i] );
        if( argument == "-h" )
            host = argv[i + 1]; // $$$$ MCO : this could crash : use Boost.CommandLine !
        else if( argument == "-p" )
            port = argv[i + 1];
        else if( argument == "-u" )
            profile = argv[i + 1];
        else if( argument == "-a" )
            password = argv[i + 1];
    }
    try
    {
        Client client( host + ":" + port, profile, password );
        while( true )
            client.Update();
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch( ... )
    {
        std::cerr << "unknown exception" << std::endl;
    }
    return EXIT_FAILURE;
}
