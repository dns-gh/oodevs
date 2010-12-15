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
#include "launcher_dll/LauncherFacade.h"
#include "protocol/LauncherSenders.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( push, 0 )
#include <qapplication.h>
#pragma warning( pop )

namespace
{
    const unsigned short defaultPort = 33000;
    const std::string    defaultHost = "127.0.0.1";
    const unsigned int   timeOut     = 1000;

    int   argc = 2;
    char* argv[] = { "", "--root-dir=../../data" };
    QApplication dummyApplication( argc, argv );

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
}

// -----------------------------------------------------------------------------
// Name: FrontendConfigurationMatches
// Created: SBO 2010-11-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( FrontendConfigurationMatches )
{
    frontend::Config frontendConfig;
    BOOST_REQUIRE_EQUAL( defaultPort, frontendConfig.GetLauncherPort() );
}

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : launcher( qApp->argc(), qApp->argv() )
            , client  ( controllers.controller_ )
            , timeout ( timeOut )
        {
            MOCK_EXPECT( handler, OnConnectionSucceeded ).once();
            client.Connect( defaultHost, defaultPort, handler );
            while( !client.Connected() && !timeout.Expired() )
            {
                client.Update();
                launcher.Update();
            }
        }
        LauncherFacade launcher;
        kernel::Controllers controllers;
        frontend::LauncherClient client;
        MockConnectionHandler handler;
        Timeout timeout;
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
        client.Update();
        launcher.Update();
    }
    BOOST_CHECK( listener.Check() );
}

namespace
{
    MOCK_BASE_CLASS( MockResponseHandler, frontend::ResponseHandler_ABC )
    {
        MOCK_METHOD_EXT( Handle, 1, void( const sword::ExercicesListResponse& ), HandleExercisesListResponse );
        MOCK_METHOD_EXT( Handle, 1, void( const sword::ControlStartExerciseAck& ), HandleControlStartAck );
        MOCK_METHOD_EXT( Handle, 1, void( const sword::ControlStopExerciseAck& ), HandleControlStopAck );
        MOCK_METHOD_EXT( Handle, 1, void( const sword::ProfileDescriptionList& ), HandleProfileDescriptionList );
    };
}

// -----------------------------------------------------------------------------
// Name: ClientCanStartExercise
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
// $$$$ MCO : this seems to deadlock because for some reason the simulation processus isn't started (but shouldn't deadlock anyway...)
//BOOST_FIXTURE_TEST_CASE( ClientCanStartExercise, Fixture )
//{
//    BOOST_REQUIRE( client.Connected() );
//    ExerciseListener listener( controllers );
//    client.QueryExerciseList();
//    timeout.Start();
//    while( !listener.Check() && !timeout.Expired() )
//    {
//        client.Update();
//        launcher.Update();
//    }
//    BOOST_REQUIRE( listener.Check() );
//    {
//        const frontend::Exercise_ABC* exercise = listener.exercises_.front();
//        BOOST_REQUIRE( exercise );
//        exercise->Start( "default" );
//        timeout.Start();
//        while( !exercise->IsRunning() && !timeout.Expired() )
//        {
//            client.Update();
//            launcher.Update();
//        }
//        BOOST_REQUIRE( exercise->IsRunning() );
//    }
//    {
//        boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
//        MOCK_EXPECT( handler, HandleProfileDescriptionList ).once();
//        client.Register( handler );
//        client.QueryProfileList();
//        timeout.Start();
//        while( !timeout.Expired() )
//        {
//            client.Update();
//            launcher.Update();
//        }
//    }
//}
