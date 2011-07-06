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

BOOST_FIXTURE_TEST_CASE( aar_information_message_to_client_is_converted, ContextFixture< sword::AarToClient > )
{
    content.mutable_aar_information()->set_information( "information" );
    MOCK_EXPECT( client, SendAarToClient ).once().with( constraint( msg, "context: 42 message { aar_information { information: \"information\" } }" ) );
    converter.ReceiveAarToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( plot_result_message_to_client_is_converted, ContextFixture< sword::AarToClient > )
{
    content.mutable_plot_result()->set_identifier( 17 );
    content.mutable_plot_result()->add_values( 3 );
    content.mutable_plot_result()->add_values( 5 );
    content.mutable_plot_result()->set_error( "error" );
    MOCK_EXPECT( client, SendAarToClient ).once().with( constraint( msg, "context: 42 message { plot_result { identifier: 17 values: 3 values: 5 error: \"error\" } }" ) );
    converter.ReceiveAarToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( indicator_message_to_client_is_converted, ContextFixture< sword::AarToClient > )
{
    content.mutable_indicator()->set_name( "name" );
    content.mutable_indicator()->set_value( 17 );
    MOCK_EXPECT( client, SendAarToClient ).once().with( constraint( msg, "context: 42 message { indicator { name: \"name\" value: 17 } }" ) );
    converter.ReceiveAarToClient( msg );
}
