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
#include "protocol/AuthenticationSenders.h"
#include "protocol/ClientSenders.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/ServerNetworker.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/assign.hpp>

#include "Tools.h"

using namespace launcher_test;



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
        MOCK_METHOD_EXT( Handle, 1, void( const sword::SessionCommandExecutionResponse& ), HandleSessionCommandExecutionResponse );
    };
}

// -----------------------------------------------------------------------------
// Name: ClientCanStartExercise
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------

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
        MOCK_EXPECT( dispatcher, ConnectionSucceeded ).once().with( mock::retrieve( dispatcher.host ) );
        MOCK_EXPECT( dispatcher, ReceiveAuth ).once().with( mock::any, mock::retrieve( dispatcher.authMessage ) );
        exercise->StartDispatcher( "default" );
        timeout.Start();
        while( (!exercise->IsRunning() || !dispatcher.AuthenticationPerformed() ) && !timeout.Expired() )
        {
            Update();
        }
        BOOST_REQUIRE( exercise->IsRunning() );
    }
    {
        sword::ProfileListResponse launcherResponse;
        const frontend::Exercise_ABC* exercise = listener.exercises_.front();
        boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
        MOCK_EXPECT( handler, HandleProfileListResponse ).once().with( mock::retrieve( launcherResponse ) );
        client.Register( handler );
        exercise->QueryProfileList( );
        timeout.Start();
        while( !launcherResponse.IsInitialized() && !timeout.Expired() )
        {
            Update();
        }
    }
}


// -----------------------------------------------------------------------------
// Name: ClientCanPauseExercise
// Created: AHC 2010-05-19
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ClientCanPauseExercise, Fixture )
{
    static const std::string SESSION("default");

    BOOST_REQUIRE( client.Connected() );
    ExerciseListener listener( controllers );
    client.QueryExerciseList();
    timeout.Start();
    while( !listener.Check() && !timeout.Expired() )
    {
        Update();
    }
    BOOST_REQUIRE( listener.Check() );   
    const frontend::Exercise_ABC* exercise = listener.exercises_.front();
    // connect to dispatcher
    {
        BOOST_REQUIRE( exercise );
        MOCK_EXPECT( dispatcher, ConnectionSucceeded ).once().with( mock::retrieve( dispatcher.host ) );
        MOCK_EXPECT( dispatcher, ReceiveAuth ).once().with( mock::any, mock::retrieve( dispatcher.authMessage ) );
        exercise->StartDispatcher( SESSION );
        timeout.Start();
        while( (!exercise->IsRunning() || !dispatcher.AuthenticationPerformed() ) && 
                !timeout.Expired() )
        {
            Update();
        }
        BOOST_REQUIRE( exercise->IsRunning() );
    }
    // send pause request
    {
        sword::ClientToSim pauseMsg;
        MOCK_EXPECT( dispatcher, ReceiveSim ).once().with( mock::any , mock::retrieve( pauseMsg ) );        
        exercise->Pause( SESSION );
        timeout.Start();
        while( !pauseMsg.IsInitialized()&& !timeout.Expired() )
        {
            Update();
        }
        LAUNCHER_CHECK_MESSAGE( pauseMsg, "context: 1 message { control_pause { } }" );
    }
    // retrieve pause response
    /*{
        sword::SessionCommandExecutionResponse launcherResponse;
        boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
        MOCK_EXPECT( handler, HandleSessionCommandExecutionResponse ).once().with( mock::retrieve( launcherResponse ) );
        client.Register( handler );

        client::ControlPauseAck dispatcherResponse;
        dispatcherResponse().set_error_code( sword::ControlAck::no_error );
        dispatcherResponse.Send( dispatcher, 1 );
        timeout.Start();
        while( !launcherResponse.IsInitialized() && !timeout.Expired() )
        {
            Update();
        }
        LAUNCHER_CHECK_MESSAGE( launcherResponse, "context: 1 message { control_pause { } }" );
    }*/
}
