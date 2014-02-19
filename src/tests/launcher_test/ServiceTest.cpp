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
#include <QtGui/qapplication.h>
#pragma warning( pop )

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
// Name: NotifyUnitExtension
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyUnitExtension, ExerciseFixture )
{
    client::UnitAttributes attributes;
    attributes().mutable_unit()->set_id( 42 );
    sword::Extension_Entry* entry = attributes().mutable_extension()->add_entries();
    entry->set_name( "name" );
    entry->set_value( "value" );
    attributes.Send( dispatcher, 12 );

    sword::SessionNotification launcherResponse;
    MOCK_EXPECT( handler->HandleSessionNotification ).once().with( mock::retrieve( launcherResponse ) );

    Wait( launcherResponse );
    LAUNCHER_CHECK_MESSAGE( launcherResponse, "exercise: \"" + exercise->GetName().ToUTF8() + "\" session: \"" + session.ToUTF8() + "\" notification { unit_update { unit { id: 42 } extensions { entries { name: \"name\" value: \"value\" } } } }" );
}

// -----------------------------------------------------------------------------
// Name: NotifyFormationExtension
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyFormationExtension, ExerciseFixture )
{
    client::FormationUpdate attributes;
    attributes().mutable_formation()->set_id( 42 );
    sword::Extension_Entry* entry = attributes().mutable_extension()->add_entries();
    entry->set_name( "name" );
    entry->set_value( "value" );
    attributes.Send( dispatcher, 12 );

    sword::SessionNotification launcherResponse;
    MOCK_EXPECT( handler->HandleSessionNotification ).once().with( mock::retrieve( launcherResponse ) );

    Wait( launcherResponse );
    LAUNCHER_CHECK_MESSAGE( launcherResponse, "exercise: \"" + exercise->GetName().ToUTF8() + "\" session: \"" + session.ToUTF8() + "\" notification { formation_update { formation { id: 42 } extensions { entries { name: \"name\" value: \"value\" } } } }" );
}

// -----------------------------------------------------------------------------
// Name: NotifyProfileCreation
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyProfileCreation, ExerciseFixture )
{
    authentication::ProfileCreation creation;
    creation().mutable_profile()->set_login( "login" );
    creation().mutable_profile()->set_password( "password" );
    creation().mutable_profile()->set_supervisor( true );
    creation.Send( dispatcher, 12 );

    sword::SessionNotification launcherResponse;
    MOCK_EXPECT( handler->HandleSessionNotification ).once().with( mock::retrieve( launcherResponse ) );

    Wait( launcherResponse );
    LAUNCHER_CHECK_MESSAGE( launcherResponse, "exercise: \"" + exercise->GetName().ToUTF8() + "\" session: \"" + session.ToUTF8() + "\" notification { profile_creation { profile { login: \"login\" password: \"password\" supervisor: true } } }" );
}

// -----------------------------------------------------------------------------
// Name: NotifyProfileUpdate
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyProfileUpdate, ExerciseFixture )
{
    authentication::ProfileUpdate update;
    update().set_login( "login" );
    update().mutable_profile()->set_login( "login" );
    update().mutable_profile()->set_password( "password" );
    update().mutable_profile()->set_supervisor( true );
    update.Send( dispatcher, 12 );

    sword::SessionNotification launcherResponse;
    MOCK_EXPECT( handler->HandleSessionNotification ).once().with( mock::retrieve( launcherResponse ) );

    Wait( launcherResponse );
    LAUNCHER_CHECK_MESSAGE( launcherResponse, "exercise: \"" + exercise->GetName().ToUTF8() + "\" session: \"" + session.ToUTF8() + "\" notification { profile_update { profile { login: \"login\" password: \"password\" supervisor: true } } }" );
}

// -----------------------------------------------------------------------------
// Name: NotifyControlInformation
// Created: LGY 2011-05-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( NotifyControlInformation, ExerciseFixture )
{
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

    sword::SessionParameterChangeResponse parameterResponse;
    sword::SessionStatus statusResponse;
    MOCK_EXPECT( handler->HandleSessionParameterChangeResponse ).once().with( mock::retrieve( parameterResponse ) );
    MOCK_EXPECT( handler->HandleSessionStatus ).once().with( mock::retrieve( statusResponse ) );

    timeout.Start();
    while( !( parameterResponse.IsInitialized() && statusResponse.IsInitialized() ) && !timeout.Expired() )
        Update();
    LAUNCHER_CHECK_MESSAGE( parameterResponse, "error_code: success exercise: \"" + exercise->GetName().ToUTF8() + "\" session: \"" + session.ToUTF8() + "\" checkpoint_frequency: 39 acceleration_factor: 40" );
    LAUNCHER_CHECK_MESSAGE( statusResponse, "exercise: \"" + exercise->GetName().ToUTF8() + "\" session: \"" + session.ToUTF8() + "\" status: paused" );
}
