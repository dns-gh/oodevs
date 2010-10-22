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
#include "frontend/Exercises.h"
#include "frontend/Launcher.h"
#include "frontend/LauncherClient.h"
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

    struct Timeout
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
}

// -----------------------------------------------------------------------------
// Name: ClientCanConnectToServer
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientCanConnectToServer )
{
    kernel::Controllers controllers;
    frontend::Config config;
    BOOST_REQUIRE_EQUAL( defaultPort, config.GetLauncherPort() );
    frontend::Launcher server( controllers, config );
    frontend::LauncherClient client( controllers.controller_ );
    client.Connect( defaultHost, defaultPort );
    Timeout timeout( timeOut );
    while( !client.Connected() && !timeout.Expired() )
        client.Update();
    BOOST_REQUIRE( client.Connected() );
}

namespace
{
    class ExerciseListener : public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< frontend::Exercises >
    {
    public:
        explicit ExerciseListener( kernel::Controllers& controllers )
            : controllers_( controllers )
            , exercises_( 0 )
        {
            controllers_.Register( *this );
        }
        virtual ~ExerciseListener()
        {
            controllers_.Unregister( *this );
        }
        virtual void NotifyUpdated( const frontend::Exercises& exercises )
        {
            exercises_ = &exercises;
        }
        bool Check() const
        {
            return exercises_;
        }
        kernel::Controllers& controllers_;
        const frontend::Exercises* exercises_;
    };
}

// -----------------------------------------------------------------------------
// Name: ClientCanListAvailableExercises
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientCanListAvailableExercises )
{
    kernel::Controllers controllers;
    frontend::Config config;
    frontend::Launcher server( controllers, config );
    frontend::LauncherClient client( controllers.controller_ );
    client.Connect( defaultHost, defaultPort );
    Timeout timeout( timeOut );
    {
        while( !client.Connected() && !timeout.Expired() )
            client.Update();
        BOOST_REQUIRE( client.Connected() );
    }
    {
        ExerciseListener listener( controllers );
        client.QueryExerciseList();
        timeout.Start();
        while( !listener.Check() && !timeout.Expired() )
        {
            client.Update();
            server.Update();
        }
        BOOST_REQUIRE( listener.Check() );
    }
}
