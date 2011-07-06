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

using namespace shield;

BOOST_FIXTURE_TEST_CASE( control_replay_information_to_client_is_converted, ContextFixture< sword::ReplayToClient > )
{
    content.mutable_control_replay_information()->set_current_tick( 7 );
    content.mutable_control_replay_information()->mutable_initial_date_time()->set_data( "data" );
    content.mutable_control_replay_information()->mutable_date_time()->set_data( "data2" );
    content.mutable_control_replay_information()->set_tick_duration( 3 );
    content.mutable_control_replay_information()->set_time_factor( 4 );
    content.mutable_control_replay_information()->set_status( sword::paused );
    content.mutable_control_replay_information()->set_tick_count( 5 );
    MOCK_EXPECT( client, SendReplayToClient ).once().with( constraint( msg, "context: 42 message { control_replay_information { current_tick: 7 initial_date_time { data: \"data\" } date_time { data: \"data2\" } tick_duration: 3 time_factor: 4 status: paused tick_count: 5 } }" ) );
    converter.ReceiveReplayToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_skip_to_tick_ack_to_client_is_converted, ContextFixture< sword::ReplayToClient > )
{
    content.mutable_control_skip_to_tick_ack()->set_tick( 7 );
    content.mutable_control_skip_to_tick_ack()->set_error_code( sword::ControlAck::error_not_started );
    MOCK_EXPECT( client, SendReplayToClient ).once().with( constraint( msg, "context: 42 message { control_skip_to_tick_ack { tick: 7 error_code: error_not_started } }" ) );
    converter.ReceiveReplayToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_stop_ack_to_client_is_converted, ContextFixture< sword::ReplayToClient > )
{
    content.mutable_control_stop_ack()->set_error_code( sword::ControlAck::error_not_started );
    MOCK_EXPECT( client, SendReplayToClient ).once().with( constraint( msg, "context: 42 message { control_stop_ack { error_code: error_not_started } }" ) );
    converter.ReceiveReplayToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_pause_ack_to_client_is_converted, ContextFixture< sword::ReplayToClient > )
{
    content.mutable_control_pause_ack()->set_error_code( sword::ControlAck::error_not_started );
    MOCK_EXPECT( client, SendReplayToClient ).once().with( constraint( msg, "context: 42 message { control_pause_ack { error_code: error_not_started } }" ) );
    converter.ReceiveReplayToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_resume_ack_to_client_is_converted, ContextFixture< sword::ReplayToClient > )
{
    content.mutable_control_resume_ack()->set_error_code( sword::ControlAck::error_not_started );
    MOCK_EXPECT( client, SendReplayToClient ).once().with( constraint( msg, "context: 42 message { control_resume_ack { error_code: error_not_started } }" ) );
    converter.ReceiveReplayToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_change_time_factor_ack_to_client_is_converted, ContextFixture< sword::ReplayToClient > )
{
    content.mutable_control_change_time_factor_ack()->set_time_factor( 7 );
    content.mutable_control_change_time_factor_ack()->set_error_code( sword::ControlAck::error_not_started );
    MOCK_EXPECT( client, SendReplayToClient ).once().with( constraint( msg, "context: 42 message { control_change_time_factor_ack { time_factor: 7 error_code: error_not_started } }" ) );
    converter.ReceiveReplayToClient( msg );
}
