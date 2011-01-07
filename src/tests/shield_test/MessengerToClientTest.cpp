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

BOOST_FIXTURE_TEST_CASE( limit_creation_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_limit_creation()->mutable_id()->set_id( 12 );
    FillTacticalLine( content.mutable_limit_creation()->mutable_tactical_line() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { limit_creation { id { id: 12 } tactical_line { name: \"name\" geometry { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } diffusion { automat { id: 12 } formation { id: 13 } } } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( limit_update_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_limit_update()->mutable_id()->set_id( 12 );
    FillTacticalLine( content.mutable_limit_update()->mutable_tactical_line() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { limit_update { id { id: 12 } tactical_line { name: \"name\" geometry { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } diffusion { automat { id: 12 } formation { id: 13 } } } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( limit_destruction_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_limit_destruction()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { limit_destruction { id { id: 12 } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( limit_creation_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_limit_creation_request_ack()->set_error_code( sword::TacticalLineAck::error_invalid_geometry );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { limit_creation_request_ack { error_code: error_invalid_geometry } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( limit_destruction_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_limit_destruction_request_ack()->set_error_code( sword::TacticalLineAck::error_invalid_geometry );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { limit_destruction_request_ack { error_code: error_invalid_geometry } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( limit_update_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_limit_update_request_ack()->set_error_code( sword::TacticalLineAck::error_invalid_geometry );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { limit_update_request_ack { error_code: error_invalid_geometry } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( phase_line_creation_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_phase_line_creation()->mutable_id()->set_id( 12 );
    FillTacticalLine( content.mutable_phase_line_creation()->mutable_tactical_line() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { lima_creation { id { id: 12 } tactical_line { name: \"name\" geometry { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } diffusion { automat { id: 12 } formation { id: 13 } } } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( phase_line_update_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_phase_line_update()->mutable_id()->set_id( 12 );
    FillTacticalLine( content.mutable_phase_line_update()->mutable_tactical_line() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { lima_update { id { id: 12 } tactical_line { name: \"name\" geometry { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } diffusion { automat { id: 12 } formation { id: 13 } } } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( phase_line_destruction_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_phase_line_destruction()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { lima_destruction { id { id: 12 } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( phase_line_creation_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_phase_line_creation_request_ack()->set_error_code( sword::TacticalLineAck::error_invalid_geometry );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { lima_creation_request_ack { error_code: error_invalid_geometry } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( phase_line_destruction_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_phase_line_destruction_request_ack()->set_error_code( sword::TacticalLineAck::error_invalid_geometry );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { lima_destruction_request_ack { error_code: error_invalid_geometry } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( phase_line_update_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_phase_line_update_request_ack()->set_error_code( sword::TacticalLineAck::error_invalid_geometry );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { lima_update_request_ack { error_code: error_invalid_geometry } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( intelligence_creation_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_intelligence_creation()->mutable_id()->set_id( 12 );
    FillIntelligence( content.mutable_intelligence_creation()->mutable_intelligence() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { intelligence_creation { id { id: 12 } intelligence { name: \"name\" nature: \"nature\" level: ii embarked: true location { latitude: 17.23 longitude: 23.17 } diplomacy: friend_diplo formation { id: 77 } } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( intelligence_update_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_intelligence_update()->mutable_id()->set_id( 12 );
    FillIntelligence( content.mutable_intelligence_update() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { intelligence_update { id { id: 12 } formation { id: 77 } name: \"name\" nature: \"nature\" level: ii embarked: true location { latitude: 17.23 longitude: 23.17 } diplomacy: friend_diplo } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( intelligence_destruction_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_intelligence_destruction()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { intelligence_destruction { id { id: 12 } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( shape_creation_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_creation()->mutable_id()->set_id( 12 );
    FillShape( content.mutable_shape_creation()->mutable_shape() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_creation { id { id: 12 } shape { category: \"category\" color: \"color\" pattern: \"pattern\" points { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( shape_update_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_update()->mutable_id()->set_id( 12 );
    content.mutable_shape_update()->set_category( "category" );
    content.mutable_shape_update()->set_color( "color" );
    content.mutable_shape_update()->set_pattern( "pattern" );
    FillCoordLatLong( content.mutable_shape_update()->mutable_points()->add_elem() );
    FillCoordLatLong( content.mutable_shape_update()->mutable_points()->add_elem() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_update { id { id: 12 } category: \"category\" color: \"color\" template: \"pattern\" points { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( shape_destruction_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_destruction()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_destruction { id { id: 12 } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( shape_creation_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_creation_request_ack()->set_error_code( sword::ShapeRequestAck::error_invalid_oid );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_creation_request_ack { error_code: error_invalid_oid } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( shape_destruction_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_destruction_request_ack()->set_error_code( sword::ShapeRequestAck::error_invalid_oid );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_destruction_request_ack { error_code: error_invalid_oid } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( shape_update_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_shape_update_request_ack()->set_error_code( sword::ShapeRequestAck::error_invalid_oid );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { shape_update_request_ack { error_code: error_invalid_oid } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( text_message_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_text_message()->mutable_source()->set_profile( "source" );
    content.mutable_text_message()->mutable_target()->set_profile( "target" );
    content.mutable_text_message()->set_message( "message" );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { text_message { source { profile: \"source\" } target { profile: \"target\" } message: \"message\" } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( marker_creation_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_marker_creation()->mutable_marker()->set_id( 17 );
    content.mutable_marker_creation()->mutable_date()->set_data( "data" );
    FillMarker( content.mutable_marker_creation()->mutable_definition() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { marker_creation { marker { id: 17 } definition { name: \"name\" description: \"description\" parent { id: 23 } number: \"number\" } date { data: \"data\" } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( marker_update_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_marker_update()->mutable_marker()->set_id( 17 );
    content.mutable_marker_update()->mutable_date()->set_data( "data" );
    FillMarker( content.mutable_marker_update() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { marker_update { marker { id: 17 } date { data: \"data\" } name: \"name\" description: \"description\" parent { id: 23 } number: \"number\" } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( marker_destruction_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_marker_destruction()->mutable_marker()->set_id( 17 );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { marker_destruction { marker { id: 17 } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_creation_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_creation()->mutable_object()->set_id( 12 );
    FillClientObject( content.mutable_client_object_creation() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_creation { object { id: 12 } name: \"name\" properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_update_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_update()->mutable_object()->set_id( 12 );
    FillClientObject( content.mutable_client_object_update() );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_update { object { id: 12 } name: \"name\" properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_destruction_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_destruction()->mutable_object()->set_id( 12 );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_destruction { object { id: 12 } } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_creation_request_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_creation_ack()->set_name( "name" );
    content.mutable_client_object_creation_ack()->set_error_code( sword::ClientObjectAck::duplicate_attribute_name );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_creation_ack { name: \"name\" error_code: duplicate_attribute_name } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_destruction_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_destruction_ack()->mutable_object()->set_id( 12 );
    content.mutable_client_object_destruction_ack()->set_error_code( sword::ClientObjectAck::duplicate_attribute_name );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_destruction_ack { object { id: 12 } error_code: duplicate_attribute_name } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_update_ack_to_client_is_converted, ContextFixture< sword::MessengerToClient > )
{
    content.mutable_client_object_update_ack()->mutable_object()->set_id( 12 );
    content.mutable_client_object_update_ack()->set_error_code( sword::ClientObjectAck::duplicate_attribute_name );
    MOCK_EXPECT( client, SendMessengerToClient ).once().with( constraint( msg, "context: 42 message { client_object_update_ack { object { id: 12 } error_code: duplicate_attribute_name } }" ) );
    converter.ReceiveMessengerToClient( "unused endpoint", msg );
}
