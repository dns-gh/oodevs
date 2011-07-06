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

BOOST_FIXTURE_TEST_CASE( plot_request_from_client_is_converted, ContextFixture< MsgsClientToAar::MsgClientToAar > )
{
    content.mutable_plot_request()->set_identifier( 17 );
    content.mutable_plot_request()->set_request( "request" );
    MOCK_EXPECT( server, SendClientToAar ).once().with( constraint( msg, "context: 42 message { plot_request { identifier: 17 request: \"request\" } }" ) );
    converter.ReceiveClientToAar( msg );
}
