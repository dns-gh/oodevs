// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_test_pch.h"
#include "clients_kernel/Controllers.h"
#include "frontend/Config.h"
#include "frontend/ConnectionHandler_ABC.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/LauncherClient.h"
#include "launcher_dll/LauncherFacade.h"
#include "protocol/LauncherSenders.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( push, 0 )
#include <qapplication.h>
#pragma warning( pop )

namespace
{
    const unsigned short defaultPort = 33000;
    const std::string    defaultHost = "127.0.0.1";
    const unsigned int   timeOut     = 1000;

    int   argc = 1;
    char* argv = "";
    QApplication dummyApplication( argc, &argv );

    struct Timeout : private boost::noncopyable
    {
        explicit Timeout( unsigned int duration ) : duration_( duration ) { Start(); }
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

// -----------------------------------------------------------------------------
// Name: ClientCanConnectToServer
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientCanConnectToServer )
{
    LauncherFacade launcher( qApp->argc(), qApp->argv() );
    BOOST_REQUIRE( launcher.IsInitialized() );
    kernel::Controllers controllers;
    frontend::LauncherClient client( frontend::Config(), controllers.controller_ );
    MockConnectionHandler handler;
    MOCK_EXPECT( handler, OnConnectionSucceeded ).once();
    client.Connect( defaultHost, defaultPort, handler );
    Timeout timeout( timeOut );
    while( !client.Connected() && !timeout.Expired() )
    {
        client.Update();
        launcher.Update();
    }
    BOOST_REQUIRE( client.Connected() );
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
            , hasExercise_( false )
        {
            controllers_.Register( *this );
        }
        virtual ~ExerciseListener()
        {
            controllers_.Unregister( *this );
        }
        virtual void NotifyCreated( const frontend::Exercise_ABC& /*exercise*/ )
        {
            hasExercise_ = true;
        }
        bool Check() const
        {
            return hasExercise_;
        }
        kernel::Controllers& controllers_;
        bool hasExercise_;
    };
}

// -----------------------------------------------------------------------------
// Name: ClientCanListAvailableExercises
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientCanListAvailableExercises )
{
    LauncherFacade launcher( qApp->argc(), qApp->argv() );
    BOOST_REQUIRE( launcher.IsInitialized() );
    kernel::Controllers controllers;
    frontend::LauncherClient client( frontend::Config(), controllers.controller_ );
    MockConnectionHandler handler;
    MOCK_EXPECT( handler, OnConnectionSucceeded ).once();
    client.Connect( defaultHost, defaultPort, handler );
    Timeout timeout( timeOut );
    {
        while( !client.Connected() && !timeout.Expired() )
        {
            client.Update();
            launcher.Update();
        }
        BOOST_REQUIRE( client.Connected() );
    }
    {
        ExerciseListener listener( controllers );
        client.QueryExerciseList();
        timeout.Start();
        while( !listener.Check() && !timeout.Expired() )
        {
            client.Update();
            launcher.Update();
        }
        BOOST_REQUIRE( listener.Check() );
    }
}
