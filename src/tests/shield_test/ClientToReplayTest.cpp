// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "shield_test_pch.h"
#include "ClientTools.h"

using namespace shield;

BOOST_FIXTURE_TEST_CASE( control_skip_to_tick_from_client_is_converted, ContextFixture< MsgsClientToReplay::MsgClientToReplay > )
{
    content.mutable_control_skip_to_tick()->set_tick( 12 );
    MOCK_EXPECT( server, SendClientToReplay ).once().with( constraint( msg, "context: 42 message { control_skip_to_tick { tick: 12 } }" ) );
    converter.ReceiveClientToReplay( msg );
}

BOOST_FIXTURE_TEST_CASE( control_change_time_factor_from_client_is_converted, ContextFixture< MsgsClientToReplay::MsgClientToReplay > )
{
    content.mutable_control_change_time_factor()->set_time_factor( 12 );
    MOCK_EXPECT( server, SendClientToReplay ).once().with( constraint( msg, "context: 42 message { control_change_time_factor { time_factor: 12 } }" ) );
    converter.ReceiveClientToReplay( msg );
}

BOOST_FIXTURE_TEST_CASE( control_stop_from_client_is_converted, ContextFixture< MsgsClientToReplay::MsgClientToReplay > )
{
    content.mutable_control_stop();
    MOCK_EXPECT( server, SendClientToReplay ).once().with( constraint( msg, "context: 42 message { control_stop { } }" ) );
    converter.ReceiveClientToReplay( msg );
}

BOOST_FIXTURE_TEST_CASE( control_pause_from_client_is_converted, ContextFixture< MsgsClientToReplay::MsgClientToReplay > )
{
    content.mutable_control_pause();
    MOCK_EXPECT( server, SendClientToReplay ).once().with( constraint( msg, "context: 42 message { control_pause { } }" ) );
    converter.ReceiveClientToReplay( msg );
}

BOOST_FIXTURE_TEST_CASE( control_resume_from_client_is_converted, ContextFixture< MsgsClientToReplay::MsgClientToReplay > )
{
    content.mutable_control_resume();
    MOCK_EXPECT( server, SendClientToReplay ).once().with( constraint( msg, "context: 42 message { control_resume { } }" ) );
    converter.ReceiveClientToReplay( msg );
}
