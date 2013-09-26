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

bool EncodeDecode( const after::Root& input )
{
    if( !input.IsInitialized() )
        throw std::logic_error( "input message is not initialized" );
    std::vector< google::protobuf::uint8 > buffer( input.ByteSize() );
    if( !input.SerializeWithCachedSizesToArray( &buffer[0] ) )
        throw std::logic_error( "could not serialize input message" );
    before::Root output;
    return output.ParseFromArray( &buffer[0], static_cast< int >( buffer.size() ));
}

}  // namespace

// C++ fails to decode unknown enum values (while Go has no issue with it)
BOOST_AUTO_TEST_CASE( compat_extra_enum_value )
{
    after::Root msg;
    msg.mutable_enum_new_value_msg()->set_value( static_cast< after::EnumNewValue>( 0 ));
    BOOST_CHECK( EncodeDecode( msg ) );

    msg.mutable_enum_new_value_msg()->set_value( static_cast< after::EnumNewValue>( 1 ));
    BOOST_CHECK( !EncodeDecode( msg ) );
}

