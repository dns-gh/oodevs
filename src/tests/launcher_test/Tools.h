// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#pragma warning( push, 0 )
#include <qapplication.h>
#pragma warning( pop )
#include "clients_kernel/Controllers.h"
#include "frontend/Config.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/LauncherClient.h"
#include "frontend/CommandLineTools.h"
#include "launcher_dll/LauncherFacade.h"
#include "protocol/LauncherSenders.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/ClientSenders.h"
#include "MockResponseHandler.h"
#include "MockConnectionHandler.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/ServerNetworker.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/assign.hpp>

#define LAUNCHER_CHECK_MESSAGE( MSG, EXPECTED ) BOOST_CHECK_EQUAL( MSG.ShortDebugString(), EXPECTED )

namespace launcher_test
{
    const std::string  defaultHost = "127.0.0.1";
    const unsigned int timeOut     = 5000;

    struct Timeout : private boost::noncopyable
    {
        explicit Timeout( unsigned int duration )
            : duration_( duration )
        {
            Start();
        }
        void Start()
        {
            start_ = boost::posix_time::microsec_clock::universal_time();
        }
        bool Expired() const
        {
            return ( boost::posix_time::microsec_clock::universal_time() - start_ ).total_milliseconds() > duration_;
        }
        const unsigned int duration_;
        boost::posix_time::ptime start_;
    };

    MOCK_BASE_CLASS( MockDispatcher, tools::ServerNetworker )
    {
        MockDispatcher( unsigned short port )
            : tools::ServerNetworker( port )
            , authPerformed         ( false )
        {
            AllowConnections();
            RegisterMessage< MockDispatcher, sword::ClientToSim >( *this, &MockDispatcher::Receive );
            RegisterMessage< MockDispatcher, sword::ClientToAuthentication >( *this, &MockDispatcher::Receive );
        }
        MOCK_METHOD_EXT( Receive, 2, void( const std::string&, const sword::ClientToSim& ), ReceiveSim );
        MOCK_METHOD_EXT( ConnectionSucceeded, 1, void( const std::string& ), ConnectionSucceeded );
        MOCK_METHOD_EXT( ConnectionFailed, 2, void( const std::string&, const std::string& ), ConnectionFailed );
        MOCK_METHOD_EXT( ConnectionError, 2, void( const std::string&, const std::string& ), ConnectionError );

        template <typename M>
        void Send( M& msg )
        {
            ServerNetworker::Send( host, msg );
        }

        bool AuthenticationPerformed() const
        {
            return authPerformed;
        }
        void Receive( const std::string& /*endpoint*/, const sword::ClientToAuthentication& msg )
        {
            if( msg.message().has_authentication_request() )
            {
                authentication::AuthenticationResponse response;
                *response().mutable_server_version() = msg.message().authentication_request().version();
                response().set_error_code( sword::AuthenticationResponse::success );
                response.Send( *this, msg.has_context() ? msg.context() : 1 );
                authPerformed = true;
            }
        }
        std::string host;
        bool authPerformed;
    };

    struct ApplicationFixture
    {
        ApplicationFixture()
            : varg( MakeArg() )
            , args( boost::assign::list_of< char* >( "" )( "--root-dir=../../data" )( "--test" )( &varg[0] ) )
            , argc( args.size() )
            , app ( argc, &args[0] )
        {}
        std::vector< char > MakeArg()
        {
            const std::string arg( "--launcher-port=" + boost::lexical_cast< std::string >( PORT ) );
            std::vector< char > result( arg.begin(), arg.end() );
            result.push_back( 0 );
            return result;
        }
        std::string arg;
        std::vector< char > varg;
        std::vector< char* > args;
        int argc;
        QApplication app;
    };

    class ExerciseListener : private boost::noncopyable
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< frontend::Exercise_ABC >
    {
    public:
        explicit ExerciseListener( kernel::Controllers& controllers )
            : controllers_( controllers )
        {
            controllers_.Register( *this );
        }
        virtual ~ExerciseListener()
        {
            controllers_.Unregister( *this );
        }
        virtual void NotifyCreated( const frontend::Exercise_ABC& exercise )
        {
            exercises_.push_back( &exercise );
        }
        bool Check() const
        {
            return exercises_.size() > 0;
        }
        kernel::Controllers& controllers_;
        std::vector< const frontend::Exercise_ABC* > exercises_;
    };

    struct Fixture : ApplicationFixture
    {
        Fixture()
            : client    ( controllers.controller_ )
            , timeout   ( timeOut )
            , dispatcher( frontend::DispatcherPort( 1 ) )
        {
            launcher.Initialize( argc, &args[0] );
            BOOST_REQUIRE_MESSAGE( launcher.GetLastError().empty(), launcher.GetLastError() );
            MOCK_EXPECT( handler, OnConnectionSucceeded ).once();
            client.Connect( defaultHost, PORT, handler );
            while( !client.Connected() && !timeout.Expired() )
            {
                Update();
            }
        }
        void Update()
        {
            client.Update();
            launcher.Update();
            dispatcher.Update();
        }

        LauncherFacade launcher;
        kernel::Controllers controllers;
        frontend::LauncherClient client;
        MockConnectionHandler handler;
        Timeout timeout;
        MockDispatcher dispatcher;
    };

    struct ExerciseFixture : Fixture
    {
        ExerciseFixture() 
            : Fixture()
            , exerciceListener( controllers )
            , SESSION         ( "default" )
        {
            BOOST_REQUIRE( client.Connected() );
            
            client.QueryExerciseList();
            timeout.Start();
            while( !exerciceListener.Check() && !timeout.Expired() )
            {
                Update();
            }

            BOOST_REQUIRE( exerciceListener.Check() );
            exercise = exerciceListener.exercises_.front();
            BOOST_REQUIRE( exercise );
            MOCK_EXPECT( dispatcher, ConnectionSucceeded ).once().with( mock::retrieve( dispatcher.host ) );
            exercise->StartDispatcher( SESSION );
            timeout.Start();
            while( (!exercise->IsRunning() || !dispatcher.AuthenticationPerformed() ) && !timeout.Expired() )
            {
                Update();
            }
            BOOST_REQUIRE( exercise->IsRunning() );
        }

        void VerifySendRequest( const std::string& expected )
        {
            sword::ClientToSim message;
            MOCK_EXPECT( dispatcher, ReceiveSim ).once().with( mock::any , mock::retrieve( message ) );
            timeout.Start();
            while( !message.IsInitialized()&& !timeout.Expired() )
                Update();
            LAUNCHER_CHECK_MESSAGE( message, expected.c_str() );
            mock::verify();
        }

        ExerciseListener exerciceListener;
        const frontend::Exercise_ABC* exercise;
        const std::string SESSION;
    };
}

