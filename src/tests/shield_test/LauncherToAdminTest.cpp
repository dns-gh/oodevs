// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "shield_test_pch.h"
#include "Tools.h"

using namespace shield;

BOOST_FIXTURE_TEST_CASE( connection_ack_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_connection_ack()->set_error_code( sword::ConnectionAck::invalid_connection );
    content.mutable_connection_ack()->set_context( 7 );
    content.mutable_connection_ack()->mutable_server_version()->set_value( "version" );
    content.mutable_connection_ack()->mutable_dispatcher_address()->set_ip( "ip" );
    content.mutable_connection_ack()->mutable_dispatcher_address()->set_port( 8 );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { connection_ack { error_code: invalid_connection context: 7 server_version { value: \"version\" } dispatcher_address { ip: \"ip\" port: 8 } } }" ) );
    converter.ReceiveLauncherToAdmin( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( exercise_list_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_exercise_list_response()->set_error_code( sword::ExercicesListResponse::failure );
    FillExercice( content.mutable_exercise_list_response()->add_exercise() );
    FillExercice( content.mutable_exercise_list_response()->add_exercise() );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { exercise_list_response { error_code: failure exercise { name: \"name\" running: true port: 7 } exercise { name: \"name\" running: true port: 7 } } }" ) );
    converter.ReceiveLauncherToAdmin( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( control_start_ack_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_control_start_ack()->set_error_code( sword::ControlStartExerciseAck::bad_exercise_name );
    FillExercice( content.mutable_control_start_ack()->mutable_exercise() );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { control_start_ack { error_code: bad_exercise_name exercise { name: \"name\" running: true port: 7 } } }" ) );
    converter.ReceiveLauncherToAdmin( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( control_stop_ack_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_control_stop_ack()->set_error_code( sword::ControlStopExerciseAck::bad_exercise_name );
    FillExercice( content.mutable_control_stop_ack()->mutable_exercise() );
    content.mutable_control_stop_ack()->set_simulation_state( sword::paused );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { control_stop_ack { error_code: bad_exercise_name exercise { name: \"name\" running: true port: 7 } simulation_state: paused } }" ) );
    converter.ReceiveLauncherToAdmin( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( connected_profile_list_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_connected_profile_list()->set_error_code( sword::ConnectedProfileList::failure );
    content.mutable_connected_profile_list()->set_context( sword::ConnectedProfileList::upon_request );
    FillProfile( content.mutable_connected_profile_list()->add_exercise() );
    FillProfile( content.mutable_connected_profile_list()->add_exercise() );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { connected_profile_list { error_code: failure context: upon_request exercise { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automates { elem { id: 8 } elem { id: 9 } } read_write_automates { elem { id: 10 } elem { id: 11 } } read_only_camps { elem { id: 12 } elem { id: 13 } } read_write_camps { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true role { id: 77 } } exercise { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automates { elem { id: 8 } elem { id: 9 } } read_write_automates { elem { id: 10 } elem { id: 11 } } read_only_camps { elem { id: 12 } elem { id: 13 } } read_write_camps { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true role { id: 77 } } } }" ) );
    converter.ReceiveLauncherToAdmin( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( profiles_description_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    FillProfileDescription( content.mutable_profiles_description()->add_elem() );
    FillProfileDescription( content.mutable_profiles_description()->add_elem() );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { profiles_description { elem { login: \"login\" password: true supervisor: true role { id: 77 } } elem { login: \"login\" password: true supervisor: true role { id: 77 } } } }" ) );
    converter.ReceiveLauncherToAdmin( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( simulation_component_state_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_simulation_component_state()->set_simulation_state( sword::paused );
    content.mutable_simulation_component_state()->set_dispatcher_state( sword::unavailable );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { simulation_component_state { simulation_state: paused dispatcher_state: unavailable } }" ) );
    converter.ReceiveLauncherToAdmin( "unused endpoint", msg );
}
