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
        Update();
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
    exercise->QueryProfileList();
    timeout.Start();
    while( !launcherResponse.IsInitialized() && !timeout.Expired() )
        Update();
}

// -----------------------------------------------------------------------------
// Name: ClientCanPauseExercise
// Created: AHC 2011-05-19
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
            Update();
        LAUNCHER_CHECK_MESSAGE( launcherResponse, "error_code: success exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" running: false" );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientCanResumeExercise
// Created: AHC 2011-05-20
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
            Update();
        LAUNCHER_CHECK_MESSAGE( launcherResponse, "error_code: session_already_running exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" running: true" );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientCanSaveCheckPoint
// Created: AHC 2011-05-20
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
            Update();
        LAUNCHER_CHECK_MESSAGE( launcherResponse, "error_code: success exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" saved_checkpoint: \"checkpoint\"" );
    }
}

// -----------------------------------------------------------------------------
// Name: NotifyUnitExtension
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyUnitExtension, ExerciseFixture )
{
    sword::SessionNotification launcherResponse;
    boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
    MOCK_EXPECT( handler, HandleSessionNotification ).once().with( mock::retrieve( launcherResponse ) );
    client.Register( handler );

    client::UnitAttributes attributes;
    attributes().mutable_unit()->set_id( 42 );
    sword::Extension_Entry* entry = attributes().mutable_extension()->add_entries();
    entry->set_name( "name" );
    entry->set_value( "value" );
    attributes.Send( dispatcher, 12 );
    timeout.Start();
    while( !launcherResponse.IsInitialized() && !timeout.Expired() )
        Update();
    LAUNCHER_CHECK_MESSAGE( launcherResponse, "exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" notification { unit_update { unit { id: 42 } extensions { entries { name: \"name\" value: \"value\" } } } }" );
}

// -----------------------------------------------------------------------------
// Name: NotifyFormationExtension
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyFormationExtension, ExerciseFixture )
{
    sword::SessionNotification launcherResponse;
    boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
    MOCK_EXPECT( handler, HandleSessionNotification ).once().with( mock::retrieve( launcherResponse ) );
    client.Register( handler );

    client::FormationUpdate attributes;
    attributes().mutable_formation()->set_id( 42 );
    sword::Extension_Entry* entry = attributes().mutable_extension()->add_entries();
    entry->set_name( "name" );
    entry->set_value( "value" );
    attributes.Send( dispatcher, 12 );
    timeout.Start();
    while( !launcherResponse.IsInitialized() && !timeout.Expired() )
        Update();
    LAUNCHER_CHECK_MESSAGE( launcherResponse, "exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" notification { formation_update { formation { id: 42 } extensions { entries { name: \"name\" value: \"value\" } } } }" );
}

// -----------------------------------------------------------------------------
// Name: NotifyProfileCreation
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyProfileCreation, ExerciseFixture )
{
    sword::SessionNotification launcherResponse;
    boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
    MOCK_EXPECT( handler, HandleSessionNotification ).once().with( mock::retrieve( launcherResponse ) );
    client.Register( handler );

    authentication::ProfileCreation creation;
    creation().mutable_profile()->set_login( "login" );
    creation().mutable_profile()->set_password( "password" );
    creation().mutable_profile()->set_supervisor( true );
    creation.Send( dispatcher, 12 );
    timeout.Start();
    while( !launcherResponse.IsInitialized() && !timeout.Expired() )
        Update();
    LAUNCHER_CHECK_MESSAGE( launcherResponse, "exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" notification { profile_creation { profile { login: \"login\" password: \"password\" supervisor: true } } }" );
}

// -----------------------------------------------------------------------------
// Name: NotifyProfileUpdate
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyProfileUpdate, ExerciseFixture )
{
    sword::SessionNotification launcherResponse;
    boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
    MOCK_EXPECT( handler, HandleSessionNotification ).once().with( mock::retrieve( launcherResponse ) );
    client.Register( handler );

    authentication::ProfileUpdate update;
    update().set_login( "login" );
    update().mutable_profile()->set_login( "login" );
    update().mutable_profile()->set_password( "password" );
    update().mutable_profile()->set_supervisor( true );
    update.Send( dispatcher, 12 );
    timeout.Start();
    while( !launcherResponse.IsInitialized() && !timeout.Expired() )
        Update();
    LAUNCHER_CHECK_MESSAGE( launcherResponse, "exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" notification { profile_update { profile { login: \"login\" password: \"password\" supervisor: true } } }" );
}

// -----------------------------------------------------------------------------
// Name: NotifyControlInformation
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyControlInformation, ExerciseFixture )
{
    sword::SessionParameterChangeResponse parameterResponse;
    sword::SessionStatus statusResponse;
    boost::shared_ptr< MockResponseHandler > handler( new MockResponseHandler() );
    MOCK_EXPECT( handler, HandleSessionParameterChangeResponse ).once().with( mock::retrieve( parameterResponse ) );
    MOCK_EXPECT( handler, HandleSessionStatus ).once().with( mock::retrieve( statusResponse ) );
    client.Register( handler );

    client::ControlInformation information;
    information().set_current_tick( 42 );
    information().mutable_initial_date_time()->set_data( "initialDate" );
    information().mutable_date_time()->set_data( "dateTime" );
    information().set_tick_duration( 41 );
    information().set_time_factor( 40 );
    information().set_checkpoint_frequency( 39 );
    information().set_status( sword::EnumSimulationState( 1 ) );
    information().set_send_vision_cones( true );
    information().set_profiling_enabled( false );
    information.Send( dispatcher, 12 );
    timeout.Start();
    while( !( parameterResponse.IsInitialized() && statusResponse.IsInitialized() ) && !timeout.Expired() )
        Update();
    LAUNCHER_CHECK_MESSAGE( parameterResponse, "error_code: success exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" checkpoint_frequency: 39 acceleration_factor: 40" );
    LAUNCHER_CHECK_MESSAGE( statusResponse, "exercise: \"" + exercise->GetName() + "\" session: \"" + SESSION + "\" status: paused" );
}
