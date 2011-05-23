// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "launcher_test_pch.h"
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

// -----------------------------------------------------------------------------
// Name: ClientCanStartExercise
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE( ClientCanStartExercise, ExerciseFixture )
{
    sword::ProfileListResponse launcherResponse;
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

// -----------------------------------------------------------------------------
// Name: ClientCanPauseExercise
// Created: AHC 2010-05-19
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ClientCanPauseExercise, ExerciseFixture )
{
    // send pause request
    exercise->Pause( SESSION );
    VerifySendRequest( "context: 1 message { control_pause { } }" );
    // retrieve pause response
    {
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
        LAUNCHER_CHECK_MESSAGE( launcherResponse, "error_code: success exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" running: false" );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientCanResumeExercise
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ClientCanResumeExercise, ExerciseFixture )
{
    // send resume request
    exercise->Resume( SESSION );
    VerifySendRequest( "context: 2 message { control_resume { } }" );
    // retrieve resume response
    {
        sword::SessionCommandExecutionResponse launcherResponse;
        boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
        MOCK_EXPECT( handler, HandleSessionCommandExecutionResponse ).once().with( mock::retrieve( launcherResponse ) );
        client.Register( handler );

        client::ControlResumeAck dispatcherResponse;
        dispatcherResponse().set_error_code( sword::ControlAck::error_not_paused );
        dispatcherResponse.Send( dispatcher, 2 );
        timeout.Start();
        while( !launcherResponse.IsInitialized() && !timeout.Expired() )
        {
            Update();
        }
        LAUNCHER_CHECK_MESSAGE( launcherResponse, "error_code: session_already_running exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" running: true" );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientCanSaveCheckPoint
// Created: AHC 2010-05-20
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ClientCanSaveCheckPoint, ExerciseFixture )
{
    // send checkpoint request
    exercise->SaveCheckpoint( SESSION, "checkpoint" );
    VerifySendRequest( "context: 0 message { control_checkpoint_save_now { name: \"checkpoint\" } }" );
    // retrieve checkpoint response
    {
        sword::SessionCommandExecutionResponse launcherResponse;
        boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
        MOCK_EXPECT( handler, HandleSessionCommandExecutionResponse ).once().with( mock::retrieve( launcherResponse ) );
        client.Register( handler );

        client::ControlCheckPointSaveEnd dispatcherResponse;
        dispatcherResponse().set_name( "checkpoint" );
        dispatcherResponse.Send( dispatcher, 0 );
        timeout.Start();
        while( !launcherResponse.IsInitialized() && !timeout.Expired() )
        {
            Update();
        }
        LAUNCHER_CHECK_MESSAGE( launcherResponse, "error_code: success exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" saved_checkpoint: \"checkpoint\"" );
    }
}

