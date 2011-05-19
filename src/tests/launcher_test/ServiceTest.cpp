// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "launcher_test_pch.h"
#include "clients_kernel/Controllers.h"
#include "frontend/Config.h"
#include "frontend/ConnectionHandler_ABC.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/LauncherClient.h"
#include "frontend/ResponseHandler_ABC.h"
#include "frontend/CommandLineTools.h"
#include "launcher_dll/LauncherFacade.h"
#include "protocol/LauncherSenders.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/ServerNetworker.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/assign.hpp>
#pragma warning( push, 0 )
#include <qapplication.h>
#pragma warning( pop )

namespace
{
    const std::string   defaultHost = "127.0.0.1";
    const unsigned int  timeOut     = 5000;

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

    MOCK_BASE_CLASS( MockConnectionHandler, frontend::ConnectionHandler_ABC )
    {
        MOCK_METHOD( OnConnectionSucceeded, 0 );
        MOCK_METHOD( OnConnectionFailed, 1 );
        MOCK_METHOD( OnConnectionLost, 1 );
        MOCK_METHOD( OnError, 1 );
    };
    MOCK_BASE_CLASS( MockDispatcher, tools::ServerNetworker )
    {
        MockDispatcher(unsigned short port)
            : tools::ServerNetworker( port )
        {
            AllowConnections();
            RegisterMessage( *this, &MockDispatcher::Receive );
        }
        MOCK_METHOD_EXT( Receive, 2, void( const std::string&, const sword::ClientToSim& ), Receive );
        MOCK_METHOD_EXT( ConnectionSucceeded, 1, void( const std::string& ), ConnectionSucceeded );
        MOCK_METHOD_EXT( ConnectionFailed, 2, void( const std::string&, const std::string& ), ConnectionFailed );
        MOCK_METHOD_EXT( ConnectionError, 2, void( const std::string&, const std::string& ), ConnectionError );
        std::string host;
    };

    struct ApplicationFixture
    {
        ApplicationFixture()
            : varg( MakeArg() )
            , args( boost::assign::list_of< char* >( "" )( "--root-dir=../../data" )("--test")( &varg[0] ) )
            , argc( args.size() )
            , app( argc, &args[0] )
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
    struct Fixture : ApplicationFixture
    {
        Fixture()
            : client  ( controllers.controller_ )
            , timeout ( timeOut )
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
}

// -----------------------------------------------------------------------------
// Name: ClientCanConnectToServer
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ClientCanConnectToServer, Fixture )
{
    BOOST_CHECK( client.Connected() );
}

namespace
{
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
}

// -----------------------------------------------------------------------------
// Name: ClientCanListAvailableExercises
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ClientCanListAvailableExercises, Fixture )
{
    BOOST_REQUIRE( client.Connected() );
    ExerciseListener listener( controllers ); // $$$$ MCO : use a mock
    client.QueryExerciseList();
    timeout.Start();
    while( !listener.Check() && !timeout.Expired() )
    {
        Update();
    }
    BOOST_CHECK( listener.Check() );
}

namespace
{
    MOCK_BASE_CLASS( MockResponseHandler, frontend::ResponseHandler_ABC )
    {
        MOCK_METHOD_EXT( Handle, 1, void( const sword::ExerciseListResponse& ), HandleExerciseListResponse );
        MOCK_METHOD_EXT( Handle, 1, void( const sword::SessionStartResponse& ), HandleSessionStartResponse );
        MOCK_METHOD_EXT( Handle, 1, void( const sword::SessionStopResponse& ), HandleSessionStopResponse );
        MOCK_METHOD_EXT( Handle, 1, void( const sword::ProfileListResponse& ), HandleProfileListResponse );
    };
}

// -----------------------------------------------------------------------------
// Name: ClientCanStartExercise
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
// $$$$ MCO : this seems to deadlock because for some reason the simulation process isn't started (but shouldn't deadlock anyway...)
BOOST_FIXTURE_TEST_CASE( ClientCanStartExercise, Fixture )
{
    BOOST_REQUIRE( client.Connected() );
    ExerciseListener listener( controllers );
    client.QueryExerciseList();
    timeout.Start();
    while( !listener.Check() && !timeout.Expired() )
    {
        Update();
    }
    BOOST_REQUIRE( listener.Check() );
    {
        const frontend::Exercise_ABC* exercise = listener.exercises_.front();
        BOOST_REQUIRE( exercise );
        exercise->StartDispatcher( "default" );
        timeout.Start();
        while( !exercise->IsRunning() && !timeout.Expired() )
        {
            Update();
        }
        BOOST_REQUIRE( exercise->IsRunning() );
    }
    {
        const frontend::Exercise_ABC* exercise = listener.exercises_.front();
        boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
        MOCK_EXPECT( handler, HandleProfileListResponse ).once();
        client.Register( handler );
        exercise->QueryProfileList( );
        timeout.Start();
        while( !timeout.Expired() )
        {
            Update();
        }
    }
}
