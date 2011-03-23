// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester/Exercise.h"
#include "client_proxy/SwordProxy.h"
#include "client_proxy/SwordConnectionHandler_ABC.h"
#include "client_proxy/SwordMessageHandler_ABC.h"
#include "actions/actionFactory.h"
#pragma warning( push )
#pragma warning( disable: 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace
{
    class MessageHandler : public SwordMessageHandler_ABC
    {
        virtual void OnReceiveMessage( const sword::SimToClient& /*message*/ )
        {
            std::cout << "simulation message received" << std::endl;
        }
        virtual void OnReceiveMessage( const sword::MessengerToClient& /*message*/ )
        {
            std::cout << "messenger message received" << std::endl;
        }
    };
    class ConnectionHandler : public SwordConnectionHandler_ABC
    {
        virtual void OnConnectionSucceeded( const std::string& /*endpoint*/ )
        {
            std::cout << "connection succeeded" << std::endl;
        }
        virtual void OnConnectionFailed( const std::string& /*endpoint*/, const std::string& /*reason*/ )
        {
            std::cout << "connection failed" << std::endl;
        }
        virtual void OnConnectionError( const std::string& /*endpoint*/, const std::string& /*reason*/ )
        {
            std::cout << "connection error" << std::endl;
        };
        virtual void OnAuthenticationSucceeded( const std::string& /*profile*/ )
        {
            std::cout << "authentication succeeded" << std::endl;
        }
        virtual void OnAuthenticationFailed( const std::string& /*profile*/, const std::string& /*reason*/ )
        {
            std::cout << "authentication failed" << std::endl;
        }
    };
    const boost::program_options::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        boost::program_options::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help"                                                    , "produce help message" )
            ( "exercise", boost::program_options::value< std::string >(), "set exercise" )
            ( "root-dir", boost::program_options::value< std::string >(), "set root directory" );
        boost::program_options::variables_map vm;
        boost::program_options::store( boost::program_options::command_line_parser( argc, argv ).options( desc ).run(), vm );
        boost::program_options::notify( vm );
        if( vm.count( "help" ) )
            std::cout << desc << std::endl;
        else if( ! vm.count( "exercise" ) )
            throw std::invalid_argument( "Invalid application option argument: missing exercise definition" );
        else if( ! vm.count( "root-dir" ) )
            throw std::invalid_argument( "Invalid application option argument: missing root directory" );
        return vm;
    }
}

int main( int argc, char* argv[] )
{
    try
    {
        const boost::program_options::variables_map vm = ParseCommandLine( argc, argv );
        if( vm.count( "help" ) )
            return EXIT_SUCCESS;
        mission_tester::Exercise exercise( vm[ "exercise" ].as< std::string >(), vm[ "root-dir" ].as< std::string >() );
        SwordProxy proxy( "localhost", 10001, "Admin", "" );
        std::auto_ptr< MessageHandler > messageHandler( new MessageHandler() );
        std::auto_ptr< ConnectionHandler > connectionHandler( new ConnectionHandler() );
        proxy.RegisterMessageHandler( messageHandler.get() );
        proxy.Connect( connectionHandler.get() );
        for( unsigned int i = 0; i < 1000; ++i )
            proxy.Update();
        proxy.Disconnect();
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}