// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "shield_test_pch.h"
#include "SimTools.h"
#include "protocol/proto/launcher_admin.pb.h"

using namespace shield;

BOOST_FIXTURE_TEST_CASE( connection_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_connection_response()->set_error_code( sword::ConnectionResponse::incompatible_protocol_version );
    content.mutable_connection_response()->mutable_server_version()->set_value( "version" );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { connection_response { error_code: incompatible_protocol_version server_version { value: \"" + Version::ProtocolVersion().value() + "\" } } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( exercise_list_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_exercise_list_response()->add_exercise("exercice1");
    content.mutable_exercise_list_response()->add_exercise("exercice2");
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { exercise_list_response { exercise: \"exercice1\" exercise: \"exercice2\" } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( session_list_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_session_list_response()->set_error_code( sword::SessionListResponse::invalid_exercise_name );
    content.mutable_session_list_response()->set_exercise("exercice1");
    content.mutable_session_list_response()->add_session("session1");
    content.mutable_session_list_response()->add_session("session2");
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { session_list_response { error_code: invalid_exercise_name exercise: \"exercice1\" session: \"session1\" session: \"session2\" } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( control_start_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_session_start_response()->set_error_code( sword::SessionStartResponse::invalid_exercise_name );
    content.mutable_session_start_response()->set_exercise("name") ;
    content.mutable_session_start_response()->set_session("session") ;
    content.mutable_session_start_response()->set_checkpoint("checkpoint") ;
    content.mutable_session_start_response()->set_type( sword::SessionStartResponse::dispatch );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { session_start_response { error_code: invalid_exercise_name exercise: \"name\" session: \"session\" checkpoint: \"checkpoint\" type: dispatch } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( session_stop_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_session_stop_response()->set_error_code( sword::SessionStopResponse::invalid_session_name );
    content.mutable_session_stop_response()->set_exercise("name") ;
    content.mutable_session_stop_response()->set_session("session") ;
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { session_stop_response { error_code: invalid_session_name exercise: \"name\" session: \"session\" } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_list_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_profile_list_response()->set_error_code( sword::ProfileListResponse::invalid_exercise_name );
    content.mutable_profile_list_response()->set_exercise("name") ;
    FillProfile( content.mutable_profile_list_response()->add_profile() ) ;
    FillProfile( content.mutable_profile_list_response()->add_profile() ) ;
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { profile_list_response { error_code: invalid_exercise_name exercise: \"name\" "
            "profile { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automata { elem { id: 8 } elem { id: 9 } } read_write_automata { elem { id: 10 } elem { id: 11 } } read_only_parties { elem { id: 12 } elem { id: 13 } } read_write_parties { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true role { id: 77 } } "
            "profile { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automata { elem { id: 8 } elem { id: 9 } } read_write_automata { elem { id: 10 } elem { id: 11 } } read_only_parties { elem { id: 12 } elem { id: 13 } } read_write_parties { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true role { id: 77 } } } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( connected_profile_list_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_connected_profile_list_response()->set_error_code( sword::ConnectedProfileListResponse::invalid_exercise_name );
    content.mutable_connected_profile_list_response()->set_exercise("name") ;
    content.mutable_connected_profile_list_response()->set_session("session") ;
    FillProfile( content.mutable_connected_profile_list_response()->add_profile() ) ;
    FillProfile( content.mutable_connected_profile_list_response()->add_profile() ) ;
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { connected_profile_list_response { error_code: invalid_exercise_name exercise: \"name\" session: \"session\" "
            "profile { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automata { elem { id: 8 } elem { id: 9 } } read_write_automata { elem { id: 10 } elem { id: 11 } } read_only_parties { elem { id: 12 } elem { id: 13 } } read_write_parties { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true role { id: 77 } } "
            "profile { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automata { elem { id: 8 } elem { id: 9 } } read_write_automata { elem { id: 10 } elem { id: 11 } } read_only_parties { elem { id: 12 } elem { id: 13 } } read_write_parties { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true role { id: 77 } } } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( session_status_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_session_status()->set_status( sword::SessionStatus::paused );
    content.mutable_session_status()->set_exercise("name") ;
    content.mutable_session_status()->set_session("session") ;
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { session_status { exercise: \"name\" session: \"session\" status: paused } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( session_status_with_breakdown_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_session_status()->set_status( sword::SessionStatus::breakdown );
    content.mutable_session_status()->set_exercise("name") ;
    content.mutable_session_status()->set_session("session") ;
    content.mutable_session_status()->set_breakdown_information("breakdown!") ;
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { session_status { exercise: \"name\" session: \"session\" status: breakdown breakdown_information: \"breakdown!\" } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( session_parameter_change_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_session_parameter_change_response()->set_error_code( sword::SessionParameterChangeResponse::session_not_running );
    content.mutable_session_parameter_change_response()->set_exercise("name") ;
    content.mutable_session_parameter_change_response()->set_session("session") ;
    content.mutable_session_parameter_change_response()->set_checkpoint_frequency ( 420 ) ;
    content.mutable_session_parameter_change_response()->set_acceleration_factor ( 42 ) ;
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { session_parameter_change_response { error_code: session_not_running exercise: \"name\" session: \"session\" checkpoint_frequency: 420 acceleration_factor: 42 } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( session_command_execution_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_session_command_execution_response()->set_error_code( sword::SessionCommandExecutionResponse::invalid_session_name );
    content.mutable_session_command_execution_response()->set_exercise("name") ;
    content.mutable_session_command_execution_response()->set_session("session") ;
    content.mutable_session_command_execution_response()->set_saved_checkpoint("checkpoint") ;
    content.mutable_session_command_execution_response()->set_running(true) ;
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { session_command_execution_response { error_code: invalid_session_name exercise: \"name\" session: \"session\" saved_checkpoint: \"checkpoint\" status: running } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( checkpoint_list_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_checkpoint_list_response()->set_error_code( sword::CheckpointListResponse::invalid_exercise_name );
    content.mutable_checkpoint_list_response()->set_exercise("name") ;
    content.mutable_checkpoint_list_response()->set_session("session") ;
    
    content.mutable_checkpoint_list_response()->add_checkpoint();
    sword::CheckpointListResponse_CheckPoint* checkpoint1 = content.mutable_checkpoint_list_response()->mutable_checkpoint( 0 );
    checkpoint1->set_name( "checkpoint1" );
    checkpoint1->set_type( sword::CheckpointListResponse_CheckPoint::automatic );

    content.mutable_checkpoint_list_response()->add_checkpoint() ;
    sword::CheckpointListResponse_CheckPoint* checkpoint2 = content.mutable_checkpoint_list_response()->mutable_checkpoint( 1 );
    checkpoint2->set_name( "checkpoint2" );
    checkpoint2->set_type( sword::CheckpointListResponse_CheckPoint::manual );

    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { checkpoint_list_response { error_code: invalid_exercise_name exercise: \"name\" session: \"session\" checkpoint { name: \"checkpoint1\" type: automatic } checkpoint { name: \"checkpoint2\" type: manual } } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( checkpoint_delete_response_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_checkpoint_delete_response()->set_error_code( sword::CheckpointDeleteResponse::invalid_exercise_name );
    content.mutable_checkpoint_delete_response()->set_exercise("name") ;
    content.mutable_checkpoint_delete_response()->set_session("session") ;
    content.mutable_checkpoint_delete_response()->add_checkpoint("checkpoint1") ;
    content.mutable_checkpoint_delete_response()->add_checkpoint("checkpoint2") ;
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { checkpoint_delete_response { error_code: invalid_exercise_name exercise: \"name\" session: \"session\" checkpoint: \"checkpoint1\" checkpoint: \"checkpoint2\" } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( session_notification_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_session_notification()->set_exercise("name") ;
    content.mutable_session_notification()->set_session("session") ;
    content.mutable_session_notification()->mutable_notification()->mutable_log_history_request_for_play()->set_profile( "profile" );
    content.mutable_session_notification()->mutable_notification()->mutable_log_history_request_for_play()->mutable_date_time()->set_data( "date" );
    content.mutable_session_notification()->mutable_notification()->mutable_log_history_request_for_replay_ack();
    auto creation = content.mutable_session_notification()->mutable_notification()->mutable_entity_creation();
    creation->set_long_name( "LongName" );
    creation->mutable_id()->mutable_automat()->set_id( 42 );
    creation->mutable_superior()->mutable_formation()->set_id( 4 );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { session_notification { exercise: \"name\" session: \"session\" notification { log_history_request_for_play { profile: \"profile\" date_time { data: \"date\" } } log_history_request_for_replay_ack { } entity_creation { long_name: \"LongName\" id { automat { id: 42 } } superior { formation { id: 4 } } } } } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( connected_clients_update_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_connected_clients_update()->set_exercise( "exo" );
    content.mutable_connected_clients_update()->set_session( "session" );
    sword::ClientConnectionNotification::ClientConnection* connection1 = content.mutable_connected_clients_update()->add_connection();
    connection1->set_connected(true); connection1->set_login("login1");
    sword::ClientConnectionNotification::ClientConnection* connection2 = content.mutable_connected_clients_update()->add_connection();
    connection2->set_connected(false); connection2->set_login("login2");
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { connected_clients_update { exercise: \"exo\" session: \"session\" connection { login: \"login1\" connected: true } connection { login: \"login2\" connected: false } } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}

BOOST_FIXTURE_TEST_CASE( tick_information_from_launcher_is_converted, ContextFixture< sword::LauncherToAdmin > )
{
    content.mutable_tick_information()->set_current_tick( 42 );
    content.mutable_tick_information()->set_tick_duration( 2 );
    content.mutable_tick_information()->set_time_factor( 12 );
    content.mutable_tick_information()->set_pathfind_request_number( 13 );
    content.mutable_tick_information()->set_last_checkpoint_build_duration( 14 );
    content.mutable_tick_information()->mutable_date_time()->set_data( "date" );
    MOCK_EXPECT( client, SendLauncherToAdmin ).once().with( constraint( msg, "context: 42 message { control_information { current_tick: 42 tick_duration: 2 time_factor: 12 pathfind_request_number: 13 last_checkpoint_build_duration: 14 date_time { data: \"date\" } } }" ) );
    converter.ReceiveLauncherToAdmin( msg );
}
