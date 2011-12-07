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

BOOST_FIXTURE_TEST_CASE( authentication_request_from_client_is_converted_when_version_check_fails, ContextFixture< MsgsClientToAuthentication::MsgClientToAuthentication > )
{
    content.mutable_authentication_request()->set_login( "login" );
    content.mutable_authentication_request()->set_password( "password" );
    content.mutable_authentication_request()->mutable_version()->set_value( "0.0.1" );
    MOCK_EXPECT( server, SendClientToAuthentication ).once().with( constraint( msg, "context: 42 message { authentication_request { login: \"login\" password: \"password\" version { value: \"0.0.0\" } } }" ) );
    converter.ReceiveClientToAuthentication( msg );
}

BOOST_FIXTURE_TEST_CASE( authentication_request_from_client_is_converted_when_version_check_succeeds, ContextFixture< MsgsClientToAuthentication::MsgClientToAuthentication > )
{
    content.mutable_authentication_request()->set_login( "login" );
    content.mutable_authentication_request()->set_password( "password" );
    content.mutable_authentication_request()->mutable_version()->set_value( Version::ProtocolVersion().value() );
    MOCK_EXPECT( server, SendClientToAuthentication ).once().with( constraint( msg, "context: 42 message { authentication_request { login: \"login\" password: \"password\" version { value: \"" + sword::ProtocolVersion().value() + "\" } } }" ) );
    converter.ReceiveClientToAuthentication( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_creation_request_from_client_is_converted, ContextFixture< MsgsClientToAuthentication::MsgClientToAuthentication > )
{
    FillProfile( content.mutable_profile_creation_request()->mutable_profile() );
    MOCK_EXPECT( server, SendClientToAuthentication ).once().with( constraint( msg, "context: 42 message { profile_creation_request { profile { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automates { elem { id: 8 } elem { id: 9 } } read_write_automates { elem { id: 10 } elem { id: 11 } } read_only_parties { elem { id: 12 } elem { id: 13 } } read_write_parties { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true } } }" ) );
    converter.ReceiveClientToAuthentication( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_update_request_from_client_is_converted, ContextFixture< MsgsClientToAuthentication::MsgClientToAuthentication > )
{
    FillProfile( content.mutable_profile_update_request()->mutable_profile() );
    content.mutable_profile_update_request()->set_login( "login" );
    MOCK_EXPECT( server, SendClientToAuthentication ).once().with( constraint( msg, "context: 42 message { profile_update_request { login: \"login\" profile { login: \"login\" password: \"password\" read_only_formations { elem { id: 3 } elem { id: 5 } } read_write_formations { elem { id: 6 } elem { id: 7 } } read_only_automates { elem { id: 8 } elem { id: 9 } } read_write_automates { elem { id: 10 } elem { id: 11 } } read_only_parties { elem { id: 12 } elem { id: 13 } } read_write_parties { elem { id: 14 } elem { id: 15 } } read_only_crowds { elem { id: 16 } elem { id: 17 } } read_write_crowds { elem { id: 18 } elem { id: 19 } } supervisor: true } } }" ) );
    converter.ReceiveClientToAuthentication( msg );
}

BOOST_FIXTURE_TEST_CASE( profile_destruction_request_from_client_is_converted, ContextFixture< MsgsClientToAuthentication::MsgClientToAuthentication > )
{
    content.mutable_profile_destruction_request()->set_login( "login" );
    MOCK_EXPECT( server, SendClientToAuthentication ).once().with( constraint( msg, "context: 42 message { profile_destruction_request { login: \"login\" } }" ) );
    converter.ReceiveClientToAuthentication( msg );
}
