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

BOOST_FIXTURE_TEST_CASE( connection_request_to_launcher_is_converted_when_version_check_fails, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_connection_request()->mutable_client_version()->set_value( "0.0.1" );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { connection_request { client_version { value: \"0.0.0\" } } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( connection_request_to_launcher_is_converted_when_version_check_succeeds, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_connection_request()->mutable_client_version()->set_value( Version::ProtocolVersion().value() );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { connection_request { client_version { value: \"" + sword::ProtocolVersion().value() + "\" } } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( session_start_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_session_start_request()->set_exercise( "name" );
    content.mutable_session_start_request()->set_session( "session" );
    content.mutable_session_start_request()->set_type( MsgsAdminToLauncher::MsgSessionStartRequest::dispatch );
    content.mutable_session_start_request()->set_checkpoint( "checkpoint" );
    MsgsAdminToLauncher::SessionParameter* param1 = content.mutable_session_start_request()->add_parameter(); param1->set_key("key1"); param1->set_value("value1");
    MsgsAdminToLauncher::SessionParameter* param2 = content.mutable_session_start_request()->add_parameter(); param2->set_key("key2"); param2->set_value("value2");
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { session_start_request { exercise: \"name\" session: \"session\" type: dispatch checkpoint: \"checkpoint\" "
            "parameter { key: \"key1\" value: \"value1\" } parameter { key: \"key2\" value: \"value2\" } } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( session_stop_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_session_stop_request()->set_exercise( "name" );
    content.mutable_session_stop_request()->set_session( "session" );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { session_stop_request { exercise: \"name\" session: \"session\" } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( exercise_list_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_exercise_list_request();
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { exercise_list_request { } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( session_list_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_session_list_request()->set_exercise( "name" );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { session_list_request { exercise: \"name\" } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_list_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_profile_list_request()->set_exercise( "name" );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { profile_list_request { exercise: \"name\" } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( connected_profile_list_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_connected_profile_list_request()->set_exercise( "name" );
    content.mutable_connected_profile_list_request()->set_session( "session" );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { connected_profile_list_request { exercise: \"name\" session: \"session\" } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( session_parameter_change_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_session_parameter_change_request()->set_exercise( "name" );
    content.mutable_session_parameter_change_request()->set_session( "session" );
    content.mutable_session_parameter_change_request()->set_checkpoint_frequency( 420 );
    content.mutable_session_parameter_change_request()->set_acceleration_factor( 42 );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { session_parameter_change_request { exercise: \"name\" session: \"session\" checkpoint_frequency: 420 acceleration_factor: 42 } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( session_command_execution_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_session_command_execution_request()->set_exercise( "name" );
    content.mutable_session_command_execution_request()->set_session( "session" );
    content.mutable_session_command_execution_request()->set_save_checkpoint( "checkpoint" );
    content.mutable_session_command_execution_request()->set_set_running( false );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { session_command_execution_request { exercise: \"name\" session: \"session\" save_checkpoint: \"checkpoint\" set_running: false } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( checkpoint_list_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_checkpoint_list_request()->set_exercise( "name" );
    content.mutable_checkpoint_list_request()->set_session( "session" );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { checkpoint_list_request { exercise: \"name\" session: \"session\" } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE( checkpoint_delete_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_checkpoint_delete_request()->set_exercise( "name" );
    content.mutable_checkpoint_delete_request()->set_session( "session" );
    content.mutable_checkpoint_delete_request()->set_checkpoint( "checkpoint" );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { checkpoint_delete_request { exercise: \"name\" session: \"session\" checkpoint: \"checkpoint\" } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

BOOST_FIXTURE_TEST_CASE(session_notification_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_session_notification()->set_exercise( "name" );
    content.mutable_session_notification()->set_session( "session" );
    content.mutable_session_notification()->mutable_notification()->mutable_directory_change();
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { session_notification { exercise: \"name\" session: \"session\" notification { directory_change { } } } }" ) );
    converter.ReceiveAdminToLauncher( msg );
}

