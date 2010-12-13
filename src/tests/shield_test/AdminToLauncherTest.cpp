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

BOOST_FIXTURE_TEST_CASE( connection_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_connection_request()->mutable_client_version()->set_value( "version" );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { connection_request { client_version { value: \"version\" } } }" ) );
    converter.ReceiveAdminToLauncher( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( control_start_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    FillExercice( content.mutable_control_start()->mutable_exercise() );
    content.mutable_control_start()->set_mode( MsgsAdminToLauncher::MsgControlStart::replay );
    content.mutable_control_start()->set_use_after_action_analysis( true );
    content.mutable_control_start()->set_use_external_systems( true );
    content.mutable_control_start()->set_checkpoint( "checkpoint" );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { control_start { exercise { name: \"name\" running: true port: 7 } mode: replay use_after_action_analysis: true use_external_systems: true checkpoint: \"checkpoint\" } }" ) );
    converter.ReceiveAdminToLauncher( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( control_stop_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    FillExercice( content.mutable_control_stop()->mutable_exercise() );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { control_stop { exercise { name: \"name\" running: true port: 7 } } }" ) );
    converter.ReceiveAdminToLauncher( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( exercise_list_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_exercise_list_request()->set_context( 7 );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { exercise_list_request { context: 7 } }" ) );
    converter.ReceiveAdminToLauncher( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( profile_list_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_profile_list_request()->set_context( 7 );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { profile_list_request { context: 7 } }" ) );
    converter.ReceiveAdminToLauncher( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( connected_profile_list_request_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_connected_profile_list_request()->set_context( 7 );
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { connected_profile_list_request { context: 7 } }" ) );
    converter.ReceiveAdminToLauncher( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( notification_to_launcher_is_converted, ContextFixture< MsgsAdminToLauncher::MsgAdminToLauncher > )
{
    content.mutable_notification();
    MOCK_EXPECT( server, SendAdminToLauncher ).once().with( constraint( msg, "context: 42 message { notification { } }" ) );
    converter.ReceiveAdminToLauncher( "client endpoint", msg );
}
