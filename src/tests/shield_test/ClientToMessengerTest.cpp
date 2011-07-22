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

namespace
{
    template< typename L >
    void FillLocation( L* l )
    {
        l->set_type( Common::MsgLocation::rectangle );
        FillCoordLatLong( l->mutable_coordinates()->add_elem() );
        FillCoordLatLong( l->mutable_coordinates()->add_elem() );
    }
}

BOOST_FIXTURE_TEST_CASE( shape_creation_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    FillShieldShape( content.mutable_shape_creation_request()->mutable_shape() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { shape_creation_request { shape { category: \"\" color { red: 4 green: 5 blue: 6 } pattern: \"external identifier\" points { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } }" ) );
    converter.ReceiveClientToMessenger( msg );
}

BOOST_FIXTURE_TEST_CASE( shape_update_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_shape_update_request()->mutable_shape()->set_id( 12 );
    content.mutable_shape_update_request()->mutable_color()->set_red( 4 );
    content.mutable_shape_update_request()->mutable_color()->set_green( 5 );
    content.mutable_shape_update_request()->mutable_color()->set_blue( 6 );
    content.mutable_shape_update_request()->mutable_location()->set_type( Common::MsgLocation::polygon );
    FillCoordLatLong( content.mutable_shape_update_request()->mutable_location()->mutable_coordinates()->add_elem() );
    FillCoordLatLong( content.mutable_shape_update_request()->mutable_location()->mutable_coordinates()->add_elem() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { shape_update_request { shape { id: 12 } color { red: 4 green: 5 blue: 6 } points { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } }" ) );
    converter.ReceiveClientToMessenger( msg );
}

BOOST_FIXTURE_TEST_CASE( shape_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_shape_destruction_request()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { shape_destruction_request { id { id: 12 } } }" ) );
    converter.ReceiveClientToMessenger( msg );
}

BOOST_FIXTURE_TEST_CASE( marker_creation_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    FillShieldMarker( content.mutable_marker_creation_request()->mutable_marker() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { marker_creation_request { marker { name: \"name\" description: \"description\" parent { id: 23 } number: \"information\" } } }" ) );
    converter.ReceiveClientToMessenger( msg );
}

BOOST_FIXTURE_TEST_CASE( marker_update_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_marker_update_request()->mutable_marker()->set_id( 17 );
    FillShieldMarker( content.mutable_marker_update_request() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { marker_update_request { marker { id: 17 } name: \"name\" number: \"information\" description: \"description\" parent { id: 23 } } }" ) );
    converter.ReceiveClientToMessenger( msg );
}

BOOST_FIXTURE_TEST_CASE( marker_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_marker_destruction_request()->mutable_marker()->set_id( 12 );
    content.mutable_marker_destruction_request()->set_delete_all( true );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { marker_destruction_request { marker { id: 12 } delete_all: true } }" ) );
    converter.ReceiveClientToMessenger( msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_creation_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    FillClientObject( content.mutable_client_object_creation_request() );
    content.mutable_client_object_creation_request()->set_persistent( true );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { client_object_creation_request { name: \"name\" properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } persistent: true } }" ) );
    converter.ReceiveClientToMessenger( msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_update_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_client_object_update_request()->mutable_object()->set_id( 17 );
    FillClientObject( content.mutable_client_object_update_request() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { client_object_update_request { object { id: 17 } name: \"name\" properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } } }" ) );
    converter.ReceiveClientToMessenger( msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_client_object_destruction_request()->mutable_object()->set_id( 17 );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { client_object_destruction_request { object { id: 17 } } }" ) );
    converter.ReceiveClientToMessenger( msg );
}
