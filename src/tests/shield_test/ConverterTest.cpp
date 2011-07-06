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

BOOST_FIXTURE_TEST_CASE( unknown_message_is_logged_then_dropped, Fixture )
{
    sword::SimToClient msg;
    msg.mutable_message()->mutable_burning_cell_request_ack()->set_error_code( sword::BurningCellRequestAck::no_error );
    MOCK_EXPECT( listener, Info ).once().with( "Shield converter dropping unknown 'sword.SimToClient.Content.burning_cell_request_ack' message" );
    converter.ReceiveSimToClient( msg );
}
