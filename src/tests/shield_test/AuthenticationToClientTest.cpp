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

BOOST_FIXTURE_TEST_CASE( authentication_response_to_client_is_converted, ContextFixture< sword::AuthenticationToClient > )
{
    content.mutable_authentication_response()->set_error_code( sword::AuthenticationResponse::invalid_login );
    FillProfile( content.mutable_authentication_response()->mutable_profile() );
    FillProfileDescription( content.mutable_authentication_response()->mutable_profiles()->add_elem() );
    FillProfileDescription( content.mutable_authentication_response()->mutable_profiles()->add_elem() );
    content.mutable_authentication_response()->mutable_server_version()->set_value( "version" );
    content.mutable_authentication_response()->mutable_restart_date_time()->set_data( "data" );
    content.mutable_authentication_response()->set_terrain_name( "terrain_name" );
    MOCK_EXPECT( client, SendAuthenticationToClient ).once().with( constraint( msg, "context: 42 message { authentication_response { error_code: invalid_login profile { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automates { elem { id: 8 } elem { id: 9 } } read_write_automates { elem { id: 10 } elem { id: 11 } } read_only_camps { elem { id: 12 } elem { id: 13 } } read_write_camps { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true role { id: 77 } } profiles { elem { login: \"login\" password: true supervisor: true role { id: 77 } } elem { login: \"login\" password: true supervisor: true role { id: 77 } } } server_version { value: \"" + Version::ProtocolVersion().value() + "\" } restart_date_time { data: \"data\" } terrain_name: \"terrain_name\" } }" ) );
    converter.ReceiveAuthenticationToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_creation_to_client_is_converted, ContextFixture< sword::AuthenticationToClient > )
{
    FillProfile( content.mutable_profile_creation()->mutable_profile() );
    MOCK_EXPECT( client, SendAuthenticationToClient ).once().with( constraint( msg, "context: 42 message { profile_creation { profile { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automates { elem { id: 8 } elem { id: 9 } } read_write_automates { elem { id: 10 } elem { id: 11 } } read_only_camps { elem { id: 12 } elem { id: 13 } } read_write_camps { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true role { id: 77 } } } }" ) );
    converter.ReceiveAuthenticationToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_creation_request_ack_to_client_is_converted, ContextFixture< sword::AuthenticationToClient > )
{
    content.mutable_profile_creation_request_ack()->set_login( "login" );
    content.mutable_profile_creation_request_ack()->set_error_code( sword::ProfileCreationRequestAck::invalid_login );
    MOCK_EXPECT( client, SendAuthenticationToClient ).once().with( constraint( msg, "context: 42 message { profile_creation_request_ack { login: \"login\" error_code: invalid_login } }" ) );
    converter.ReceiveAuthenticationToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_update_to_client_is_converted, ContextFixture< sword::AuthenticationToClient > )
{
    content.mutable_profile_update()->set_login( "login" );
    FillProfile( content.mutable_profile_update()->mutable_profile() );
    MOCK_EXPECT( client, SendAuthenticationToClient ).once().with( constraint( msg, "context: 42 message { profile_update { login: \"login\" profile { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automates { elem { id: 8 } elem { id: 9 } } read_write_automates { elem { id: 10 } elem { id: 11 } } read_only_camps { elem { id: 12 } elem { id: 13 } } read_write_camps { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true role { id: 77 } } } }" ) );
    converter.ReceiveAuthenticationToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_update_request_ack_to_client_is_converted, ContextFixture< sword::AuthenticationToClient > )
{
    content.mutable_profile_update_request_ack()->set_login( "login" );
    content.mutable_profile_update_request_ack()->set_error_code( sword::ProfileUpdateRequestAck::invalid_login );
    MOCK_EXPECT( client, SendAuthenticationToClient ).once().with( constraint( msg, "context: 42 message { profile_update_request_ack { login: \"login\" error_code: invalid_login } }" ) );
    converter.ReceiveAuthenticationToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_destruction_to_client_is_converted, ContextFixture< sword::AuthenticationToClient > )
{
    content.mutable_profile_destruction()->set_login( "login" );
    MOCK_EXPECT( client, SendAuthenticationToClient ).once().with( constraint( msg, "context: 42 message { profile_destruction { login: \"login\" } }" ) );
    converter.ReceiveAuthenticationToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_destruction_request_ack_to_client_is_converted, ContextFixture< sword::AuthenticationToClient > )
{
    content.mutable_profile_destruction_request_ack()->set_error_code( sword::ProfileDestructionRequestAck::invalid_profile );
    content.mutable_profile_destruction_request_ack()->set_login( "login" );
    MOCK_EXPECT( client, SendAuthenticationToClient ).once().with( constraint( msg, "context: 42 message { profile_destruction_request_ack { error_code: invalid_profile login: \"login\" } }" ) );
    converter.ReceiveAuthenticationToClient( msg );
}
