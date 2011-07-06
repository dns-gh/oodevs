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

namespace
{
    template< typename L >
    void FillTacticalLine( L* l )
    {
        l->set_name( "name" );
        FillLocation( l->mutable_geometry() );
        l->mutable_diffusion()->mutable_automat()->set_id( 12 );
        l->mutable_diffusion()->mutable_formation()->set_id( 13 );
    }
}

BOOST_FIXTURE_TEST_CASE( shape_creation_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_creation()->mutable_id()->set_id( 12 );
    FillShape( content.mutable_shape_creation()->mutable_shape() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_creation { id { id: 12 } shape { category: \"category\" color { red: 4 green: 5 blue: 6 } pattern: \"pattern\" points { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( shape_update_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_update()->mutable_id()->set_id( 12 );
    content.mutable_shape_update()->mutable_shape()->set_category( "category" );
    content.mutable_shape_update()->mutable_shape()->mutable_color()->set_red( 4 );
    content.mutable_shape_update()->mutable_shape()->mutable_color()->set_green( 5 );
    content.mutable_shape_update()->mutable_shape()->mutable_color()->set_blue( 6 );
    content.mutable_shape_update()->mutable_shape()->set_pattern( "pattern" );
    FillCoordLatLong( content.mutable_shape_update()->mutable_shape()->mutable_points()->add_elem() );
    FillCoordLatLong( content.mutable_shape_update()->mutable_shape()->mutable_points()->add_elem() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_update { id { id: 12 } shape { category: \"category\" color { red: 4 green: 5 blue: 6 } pattern: \"pattern\" points { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( shape_destruction_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_destruction()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_destruction { id { id: 12 } } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( shape_creation_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_creation_request_ack()->set_error_code( sword::ShapeRequestAck::error_invalid_oid );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_creation_request_ack { error_code: error_invalid_oid } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( shape_destruction_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_destruction_request_ack()->set_error_code( sword::ShapeRequestAck::error_invalid_oid );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_destruction_request_ack { error_code: error_invalid_oid } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( shape_update_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_update_request_ack()->set_error_code( sword::ShapeRequestAck::error_invalid_oid );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_update_request_ack { error_code: error_invalid_oid } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( text_message_to_client_is_ignored, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_text_message()->mutable_source()->set_profile( "source" );
    content.mutable_text_message()->mutable_target()->set_profile( "target" );
    content.mutable_text_message()->set_message( "message" );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( marker_creation_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_marker_creation()->mutable_marker()->set_id( 17 );
    content.mutable_marker_creation()->mutable_date()->set_data( "data" );
    FillMarker( content.mutable_marker_creation()->mutable_definition() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { marker_creation { marker { id: 17 } definition { name: \"name\" description: \"description\" parent { id: 23 } optional_information: \"information\" } date { data: \"data\" } } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( marker_update_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_marker_update()->mutable_marker()->set_id( 17 );
    content.mutable_marker_update()->mutable_date()->set_data( "data" );
    FillMarker( content.mutable_marker_update() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { marker_update { marker { id: 17 } date { data: \"data\" } name: \"name\" description: \"description\" parent { id: 23 } optional_information: \"information\" } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( marker_destruction_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_marker_destruction()->mutable_marker()->set_id( 17 );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { marker_destruction { marker { id: 17 } } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_creation_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_creation()->mutable_object()->set_id( 12 );
    FillClientObject( content.mutable_client_object_creation() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_creation { object { id: 12 } name: \"name\" properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_update_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_update()->mutable_object()->set_id( 12 );
    FillClientObject( content.mutable_client_object_update() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_update { object { id: 12 } name: \"name\" properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_destruction_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_destruction()->mutable_object()->set_id( 12 );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_destruction { object { id: 12 } } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_creation_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_creation_ack()->set_name( "name" );
    content.mutable_client_object_creation_ack()->set_error_code( sword::ClientObjectAck::duplicate_attribute_name );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_creation_ack { name: \"name\" error_code: duplicate_attribute_name } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_destruction_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_destruction_ack()->mutable_object()->set_id( 12 );
    content.mutable_client_object_destruction_ack()->set_error_code( sword::ClientObjectAck::duplicate_attribute_name );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_destruction_ack { object { id: 12 } error_code: duplicate_attribute_name } }" ) );
    converter.ReceiveMessengerToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_update_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_update_ack()->mutable_object()->set_id( 12 );
    content.mutable_client_object_update_ack()->set_error_code( sword::ClientObjectAck::duplicate_attribute_name );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_update_ack { object { id: 12 } error_code: duplicate_attribute_name } }" ) );
    converter.ReceiveMessengerToClient( msg );
}
