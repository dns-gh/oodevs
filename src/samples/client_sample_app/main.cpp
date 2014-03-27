// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <sstream>
#pragma warning( push, 0 )
#include "proto/client_authentication.pb.h"
#include "proto/authentication_client.pb.h"
#include "proto/aar_client.pb.h"
#include "proto/dispatcher_client.pb.h"
#include "proto/messenger_client.pb.h"
#include "proto/replay_client.pb.h"
#include "proto/simulation_client.pb.h"
#pragma warning( pop )

#define MAX_MESSAGE_SIZE ( 64 * 1024 * 1024 )
#define MAX_BUFFER_SIZE  4096

namespace sword
{
    // message identifiers
    namespace tag
    {
        const unsigned long client_authentication = 0xCA3E8777;
        const unsigned long client_messenger      = 0x9E03C338;
        const unsigned long client_simulation     = 0x1D973941;
        const unsigned long authentication_client = 0x1BE91304;
        const unsigned long aar_client            = 0xC6CDD5C6;
        const unsigned long dispatcher_client     = 0xB38DC355;
        const unsigned long messenger_client      = 0xFF2B3A46;
        const unsigned long replay_client         = 0xA85B7F22;
        const unsigned long simulation_client     = 0xD305BD09;
    }
}

namespace
{
    const int client_identifier = 42;

    // Failure notification helper
    void Fail( const std::string& message, int code = 0 )
    {
        std::stringstream ss;
        ss << message;
        if( code )
            ss << code;
        WSACleanup();
        throw std::runtime_error( ss.str() );
    }

    // Initialize Winsock
    void InitializeNetwork()
    {
        WSADATA wsaData;
        int result = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
        if( result != 0 )
            Fail( "WSAStartup failed with error: ", result );
    }

    // The client
    class SwordClient
    {
    public:
        explicit SwordClient( unsigned int log_level )
            : socket_( INVALID_SOCKET )
            , buffer_( new char[ MAX_MESSAGE_SIZE ] )
            , bytes_( 0 )
            , log_level_( log_level )
        {}

        virtual ~SwordClient()
        {
            delete[] buffer_;
        }

        bool Connect( const std::string& hostname, const std::string& port )
        {
            struct addrinfo* hosts = 0;
            struct addrinfo hints;

            ZeroMemory( &hints, sizeof( hints ) );
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            // resolve the server address and port
            int result = getaddrinfo( hostname.c_str(), port.c_str(), &hints, &hosts );
            if( result != 0 )
                Fail( "getaddrinfo failed with error: ", result );

            // attempt to connect to an address until one succeeds
            for( struct addrinfo* host = hosts; host != 0; host = host->ai_next )
            {
                // create a socket for connecting to server
                socket_ = socket( host->ai_family, host->ai_socktype, host->ai_protocol );
                if( socket_ == INVALID_SOCKET )
                    Fail( "socket failed with error", WSAGetLastError() );
                // connect to server
                if( connect( socket_, host->ai_addr, (int)host->ai_addrlen ) == SOCKET_ERROR )
                {
                    closesocket( socket_ );
                    socket_ = INVALID_SOCKET;
                    continue;
                }
                break;
            }
            freeaddrinfo( hosts );
            return Connected();
        }

        void Send( const sword::ClientToAuthentication& message )
        {
            // if not connected, fail
            if( !Connected() )
                Fail( "Not connected" );
            // if message is not properly filled, fail
            if( !message.IsInitialized() )
                Fail( "Invalid message (missing required fields)" );
            // allocate encoding buffer
            google::protobuf::uint8* buffer = new google::protobuf::uint8[ message.ByteSize() ];
            // encode message to destination buffer
            if( !message.SerializeWithCachedSizesToArray( buffer ) )
            {
                delete[] buffer;
                Fail( "Serialization failed" );
            }
            try
            {
                // create header with size and message tag
                unsigned long header[2];
                header[0] = htonl( message.GetCachedSize() + sizeof( unsigned long ) );
                header[1] = htonl( sword::tag::client_authentication );
                // send header
                if( send( socket_, reinterpret_cast< char* >( &header[0] ), 2 * sizeof( unsigned long ), 0 ) == SOCKET_ERROR )
                    Fail( "Failed to send message with error: ", WSAGetLastError() );
                // send content
                if( send( socket_, reinterpret_cast< char* >( buffer ), static_cast< int >( message.GetCachedSize() ), 0 ) == SOCKET_ERROR )
                    Fail( "Failed to send message with error: ", WSAGetLastError() );
            }
            catch( std::exception& )
            {
                closesocket( socket_ );
                throw;
            }
            delete[] buffer;
        }

        bool Receive()
        {
            char buffer[ MAX_BUFFER_SIZE ];
            // read some data from network (blocking I/O)
            int result = recv( socket_, buffer, MAX_BUFFER_SIZE, 0 );
            // if no data, close connection
            if( result == 0 )
                return false;
            // if an error occured, fail
            if( result < 0 )
                Fail( "Error receiving message: ", WSAGetLastError() );
            // if read data exceeds buffer size, fail
            if( bytes_ + result > MAX_MESSAGE_SIZE )
                Fail( "Buffer overflow" );
            // add received data to buffer
            memcpy( buffer_ + bytes_, buffer, result );
            bytes_ += result;
            // check if there is enough data to read header
            while( bytes_ >= 2 * sizeof( unsigned long ) )
            {
                // retrieve message header
                const unsigned long size = ntohl( *reinterpret_cast< unsigned long* >( &buffer_[0] ) );
                const unsigned long tag  = ntohl( *reinterpret_cast< unsigned long* >( &buffer_[4] ) );
                const unsigned long body_size = size - sizeof( unsigned long );
                // check if there is enough data to read body
                if( bytes_ >= size + sizeof( unsigned long ) )
                {
                    // process message body
                    if( tag == sword::tag::authentication_client )
                        ReadMessage< sword::AuthenticationToClient >( body_size, "Authentication message" );
                    else if( tag == sword::tag::aar_client )
                        ReadMessage< sword::AarToClient >( body_size, "After Action Review message" );
                    else if( tag == sword::tag::dispatcher_client )
                        ReadMessage< sword::DispatcherToClient >( body_size, "Dispatcher message" );
                    else if( tag == sword::tag::messenger_client )
                        ReadMessage< sword::MessengerToClient >( body_size, "Messenger message" );
                    else if( tag == sword::tag::replay_client )
                        ReadMessage< sword::ReplayToClient >( body_size, "Replay message" );
                    else if( tag == sword::tag::simulation_client )
                        ReadMessage< sword::SimToClient >( body_size, "Simulation message" );
                    else
                    {
                        std::cerr << "Received unhandled message: "
                                  << std::dec << "size=" << size
                                  << std::hex << " tag=" << tag << std::endl;
                    }
                    // remove processed message from buffer
                    const std::size_t message_size = size + sizeof( unsigned long );
                    bytes_ = bytes_ - message_size;
                    memcpy( buffer_, buffer_ + message_size, bytes_ );
                }
                else
                    break;
            }
            return true;
        }

        void Disconnect()
        {
            if( Connected() && shutdown( socket_, SD_SEND ) == SOCKET_ERROR )
            {
                closesocket( socket_ );
                std::cerr << "Shutdown failed with error: " << WSAGetLastError() << std::endl;
            }
        }

    private:
        template< typename Message >
        void ReadMessage( unsigned int size, const std::string& description )
        {
            // log message reception
            if( log_level_ == 1 )
                std::cout << "Received " << description << std::endl;
            // decode message
            Message message;
            if( ! message.ParseFromString( std::string( buffer_ + 2 * sizeof( unsigned long ), size ) ) )
                Fail( "Deserialization failed" );
            // log message content
            if( log_level_ == 2 )
                std::cout << "Received " << message.ShortDebugString() << std::endl;
        }

        bool Connected() const
        {
            return socket_ != INVALID_SOCKET;
        }

    private:
        SOCKET socket_;
        char* buffer_;
        std::size_t bytes_;
        unsigned int log_level_;
    };
}

// main entry point
int main( int argc, const char** argv )
{
    // default configuration
    std::string host = "localhost";
    std::string port = "10001";
    std::string profile = "Supervisor";
    std::string password = "";
    unsigned int log_level = 0;
    try
    {
        // read command line arguments
        if( argc == 2 && std::string( argv[1] ) == "--help" )
        {
            std::cout << "Usage: client_sample_app.exe [-h host] [-p port] [-u profile] [-a password] [-l log_level(0..2)]" << std::endl;
            return EXIT_SUCCESS;
        }
        for( int i = 1; i < argc - 1; ++i )
        {
            const std::string argument( argv[i] );
            if( argument == "-h" )
                host = argv[i + 1];
            else if( argument == "-p" )
                port = argv[i + 1];
            else if( argument == "-u" )
                profile = argv[i + 1];
            else if( argument == "-a" )
                password = argv[i + 1];
            else if( argument == "-l" )
                log_level = atoi( argv[i + 1] );
        }
        // initialize winsocks network
        InitializeNetwork();
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    // create client
    SwordClient client( log_level );
    try
    {
        // try to connect client to host
        if( !client.Connect( host, port ) )
            Fail( "Unable to connect to server" );
        // send authentication request
        {
            sword::ClientToAuthentication message;
            message.set_context( ::client_identifier );
            sword::AuthenticationRequest& request = *message.mutable_message()->mutable_authentication_request();
            request.mutable_version()->set_value( sword::ProtocolVersion().value() );
            request.set_login( profile );
            request.set_password( password );
            client.Send( message );
        }
        // receive and process messages until server ends
        while( client.Receive() )
            ::Sleep( 10 );
        // disconnect client
        client.Disconnect();
        // cleanup winsocks
        WSACleanup();
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        client.Disconnect();
        return EXIT_FAILURE;
    }
    catch( ... )
    {
        client.Disconnect();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
