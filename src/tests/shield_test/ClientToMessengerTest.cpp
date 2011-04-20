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
    template< typename L >
    void FillTacticalLine( L* l )
    {
        l->set_name( "name" );
        FillLocation( l->mutable_geometry() );
        l->mutable_diffusion()->mutable_automat()->set_id( 12 );
        l->mutable_diffusion()->mutable_formation()->set_id( 13 );
    }
    template< typename I >
    void FillShieldIntelligence( I* i )
    {
        i->set_name( "name" );
        i->set_nature( "nature" );
        i->set_level( Common::ii );
        i->set_embarked( true );
        FillCoordLatLong( i->mutable_location() );
        i->set_diplomacy( Common::friend_diplo );
        i->mutable_formation()->set_id( 77 );
    }
}

BOOST_FIXTURE_TEST_CASE( limit_creation_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    FillTacticalLine( content.mutable_limit_creation_request()->mutable_tactical_line() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { limit_creation_request { tactical_line { name: \"name\" geometry { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } diffusion { automat { id: 12 } formation { id: 13 } } } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( limit_update_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_limit_update_request()->mutable_id()->set_id( 12 );
    FillTacticalLine( content.mutable_limit_update_request()->mutable_tactical_line() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { limit_update_request { id { id: 12 } tactical_line { name: \"name\" geometry { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } diffusion { automat { id: 12 } formation { id: 13 } } } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( limit_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_limit_destruction_request()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { limit_destruction_request { id { id: 12 } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( lima_creation_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    FillTacticalLine( content.mutable_lima_creation_request()->mutable_tactical_line() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { phase_line_creation_request { tactical_line { name: \"name\" geometry { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } diffusion { automat { id: 12 } formation { id: 13 } } } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( lima_update_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_lima_update_request()->mutable_id()->set_id( 12 );
    FillTacticalLine( content.mutable_lima_update_request()->mutable_tactical_line() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { phase_line_update_request { id { id: 12 } tactical_line { name: \"name\" geometry { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } diffusion { automat { id: 12 } formation { id: 13 } } } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( lima_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_lima_destruction_request()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { phase_line_destruction_request { id { id: 12 } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( intelligence_creation_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    FillShieldIntelligence( content.mutable_intelligence_creation_request()->mutable_intelligence() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { intelligence_creation_request { intelligence { name: \"name\" nature: \"nature\" level: ii embarked: true location { latitude: 17.23 longitude: 23.17 } diplomacy: friendly formation { id: 77 } } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( intelligence_update_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_intelligence_update_request()->mutable_intelligence()->set_id( 12 );
    FillShieldIntelligence( content.mutable_intelligence_update_request() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { intelligence_update_request { intelligence { id: 12 } formation { id: 77 } name: \"name\" nature: \"nature\" level: ii embarked: true location { latitude: 17.23 longitude: 23.17 } diplomacy: friendly } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( intelligence_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_intelligence_destruction_request()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { intelligence_destruction_request { id { id: 12 } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( shape_creation_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    FillShape( content.mutable_shape_creation_request()->mutable_shape() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { shape_creation_request { shape { category: \"category\" color { red: 4 green: 5 blue: 6 } pattern: \"pattern\" points { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( shape_update_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_shape_update_request()->mutable_shape()->set_id( 12 );
    content.mutable_shape_update_request()->set_category( "category" );
    content.mutable_shape_update_request()->mutable_color()->set_red( 4 );
    content.mutable_shape_update_request()->mutable_color()->set_green( 5 );
    content.mutable_shape_update_request()->mutable_color()->set_blue( 6 );
    content.mutable_shape_update_request()->set_template_( "pattern" );
    FillCoordLatLong( content.mutable_shape_update_request()->mutable_points()->add_elem() );
    FillCoordLatLong( content.mutable_shape_update_request()->mutable_points()->add_elem() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { shape_update_request { shape { id: 12 } category: \"category\" color { red: 4 green: 5 blue: 6 } pattern: \"pattern\" points { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( shape_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_shape_destruction_request()->mutable_id()->set_id( 12 );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { shape_destruction_request { id { id: 12 } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( marker_creation_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    FillMarker( content.mutable_marker_creation_request()->mutable_marker() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { marker_creation_request { marker { name: \"name\" description: \"description\" parent { id: 23 } number: \"number\" } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( marker_update_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_marker_update_request()->mutable_marker()->set_id( 17 );
    FillMarker( content.mutable_marker_update_request() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { marker_update_request { marker { id: 17 } name: \"name\" number: \"number\" description: \"description\" parent { id: 23 } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( marker_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_marker_destruction_request()->mutable_marker()->set_id( 12 );
    content.mutable_marker_destruction_request()->set_delete_all( true );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { marker_destruction_request { marker { id: 12 } delete_all: true } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_creation_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    FillClientObject( content.mutable_client_object_creation_request() );
    content.mutable_client_object_creation_request()->set_persistent( true );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { client_object_creation_request { name: \"name\" properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } persistent: true } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_update_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_client_object_update_request()->mutable_object()->set_id( 17 );
    FillClientObject( content.mutable_client_object_update_request() );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { client_object_update_request { object { id: 17 } name: \"name\" properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } properties { name: \"property\" value { string_value: \"1\" integer_value: 2 unsigned_integer_value: 3 float_value: 4 bool_value: true } } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}

BOOST_FIXTURE_TEST_CASE( client_object_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToMessenger::MsgClientToMessenger > )
{
    content.mutable_client_object_destruction_request()->mutable_object()->set_id( 17 );
    MOCK_EXPECT( server, SendClientToMessenger ).once().with( constraint( msg, "context: 42 message { client_object_destruction_request { object { id: 17 } } }" ) );
    converter.ReceiveClientToMessenger( "client endpoint", msg );
}
