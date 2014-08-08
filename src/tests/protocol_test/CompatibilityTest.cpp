// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "protocol_test_pch.h"
#pragma warning( push, 0 )
#include "proto/before.pb.h"
#include "proto/after.pb.h"
#pragma warning( pop )

namespace
{

boost::shared_ptr< before::Root > EncodeDecode( const after::Root& input )
{
    if( !input.IsInitialized() )
        throw std::logic_error( "input message is not initialized" );
    std::vector< google::protobuf::uint8 > buffer( input.ByteSize() );
    if( !input.SerializeWithCachedSizesToArray( &buffer[0] ) )
        throw std::logic_error( "could not serialize input message" );
    boost::shared_ptr< before::Root > output( new before::Root );
    if( !output->ParseFromArray( &buffer[0], static_cast< int >( buffer.size() )))
        output.reset();
    return output;
}

}  // namespace

// C++ fails to decode unknown enum values (while Go has no issue with it)
BOOST_AUTO_TEST_CASE( compat_extra_enum_value )
{
    after::Root msg;
    msg.mutable_enum_new_value_msg()->set_value( static_cast< after::EnumNewValue>( 0 ));
    BOOST_CHECK( EncodeDecode( msg ) );

    ::google::protobuf::LogSilencer silencer;
    msg.mutable_enum_new_value_msg()->set_value( static_cast< after::EnumNewValue>( 1 ));
    BOOST_CHECK( !EncodeDecode( msg ) );
}

// Unknown values of optional enums are set to the default value or the first
// enumerated value.
BOOST_AUTO_TEST_CASE( compat_extra_enum_value_with_default )
{
    after::Root msg;
    msg.mutable_enum_new_value_msg()->set_value( static_cast< after::EnumNewValue>( 0 ));
    msg.mutable_enum_new_value_msg()->set_optional( static_cast< after::EnumNewValue>( 0 ));
    BOOST_CHECK( EncodeDecode( msg ) );

    msg.mutable_enum_new_value_msg()->set_optional( static_cast< after::EnumNewValue>( 1 ));
    BOOST_CHECK( EncodeDecode( msg ) );
}

BOOST_AUTO_TEST_CASE( compat_turning_uint_into_int )
{
    after::Root msg;
    msg.mutable_uint_to_int_msg()->set_uint_to_int( -29 );
    auto decoded = EncodeDecode( msg );
    BOOST_REQUIRE( decoded );
    BOOST_CHECK_EQUAL( uint32_t( -29 ), decoded->uint_to_int_msg().uint_to_int() );
}

BOOST_AUTO_TEST_CASE( compat_embedding_enum )
{
    after::Root msg;
    msg.mutable_embedding_enum()->set_value( after::EmbeddingEnum::value0 );
    auto decoded = EncodeDecode( msg );
    BOOST_REQUIRE( decoded );
    BOOST_CHECK_EQUAL( before::external0, decoded->embedding_enum().value() );
}

BOOST_AUTO_TEST_CASE( compat_int_to_enum )
{
    after::Root msg;
    msg.mutable_int_to_enum()->set_value( after::value1 );
    auto decoded = EncodeDecode( msg );
    BOOST_REQUIRE( decoded );
    BOOST_CHECK_EQUAL( int(after::value1), decoded->int_to_enum().value() );
}

BOOST_AUTO_TEST_CASE( compat_enum_to_int )
{
    after::Root msg;
    msg.mutable_enum_to_int()->set_value( -1 );
    auto decoded = EncodeDecode( msg );
    BOOST_REQUIRE( decoded );
    BOOST_CHECK_EQUAL( before::external1, decoded->enum_to_int().value() );
}

BOOST_AUTO_TEST_CASE( typed_list_to_untyped )
{
    after::Root msg;
    msg.mutable_typed_list_to_untyped()->add_elem()->set_id( 7 );
    msg.mutable_typed_list_to_untyped()->add_elem()->set_id( 13 );
    auto decoded = EncodeDecode( msg );
    BOOST_REQUIRE( decoded );
    BOOST_CHECK_EQUAL( msg.typed_list_to_untyped().elem_size(), decoded->typed_list_to_untyped().elem_size() );
    for( int i = 0; i < msg.typed_list_to_untyped().elem_size(); ++i )
        BOOST_CHECK_EQUAL( msg.typed_list_to_untyped().elem( i ).id(), decoded->typed_list_to_untyped().elem( i ).id() );
}
