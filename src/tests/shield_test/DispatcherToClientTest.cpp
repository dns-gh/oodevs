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

BOOST_FIXTURE_TEST_CASE( services_description_to_client_is_converted, MessageFixture< sword::DispatcherToClient > )
{
    content.mutable_services_description()->add_services( "service 1" );
    content.mutable_services_description()->add_services( "service 2" );
    MOCK_EXPECT( client, SendDispatcherToClient ).once().with( constraint( msg, "message { services_description { services: \"service 1\" services: \"service 2\" } }" ) );
    converter.ReceiveDispatcherToClient( msg );
}
