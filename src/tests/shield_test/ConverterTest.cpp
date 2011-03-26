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

BOOST_FIXTURE_TEST_CASE( message_from_client_is_not_converted_when_endpoints_do_no_match, Fixture )
{
    converter.ReceiveClientToAar( "mismatch endpoint", MsgsClientToAar::MsgClientToAar() );
    converter.ReceiveClientToAuthentication( "mismatch endpoint", MsgsClientToAuthentication::MsgClientToAuthentication() );
    converter.ReceiveClientToMessenger( "mismatch endpoint", MsgsClientToMessenger::MsgClientToMessenger() );
    converter.ReceiveClientToReplay( "mismatch endpoint", MsgsClientToReplay::MsgClientToReplay() );
    converter.ReceiveClientToSim( "mismatch endpoint", MsgsClientToSim::MsgClientToSim() );
}

BOOST_FIXTURE_TEST_CASE( unknown_message_is_logged_then_dropped, Fixture )
{
    sword::MessengerToClient msg;
    msg.mutable_message()->mutable_text_message()->mutable_source()->set_profile( "profile" );
    msg.mutable_message()->mutable_text_message()->mutable_target()->set_profile( "profile" );
    msg.mutable_message()->mutable_text_message()->set_message( "message" );
    MOCK_EXPECT( listener, Info ).once().with( "Shield converter dropping unknown 'sword.MessengerToClient.Content.text_message' message" );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}
