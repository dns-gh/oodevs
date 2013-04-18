// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_test_DeserializationTools_h
#define hla_plugin_test_DeserializationTools_h

#include <hla/Deserializer.h>
#include <hla/Serializer.h>

namespace hla
{
typedef boost::shared_ptr< Serializer_ABC > T_SerializerPtr;
}

namespace
{
    template< typename T >
    T Read( ::hla::Deserializer& deserializer )
    {
        T result;
        deserializer >> result;
        return result;
    }
    template< unsigned N >
    void Read( ::hla::Deserializer& deserializer )
    {
        for( unsigned int i = 0; i < N; ++i )
            Read< int8_t >( deserializer );
    }
    template< unsigned N >
    void ReadPadding( ::hla::Deserializer& deserializer )
    {
        for( unsigned int i = 0; i < N; ++i )
            BOOST_CHECK_EQUAL( 0, Read< int8_t >( deserializer ) );
    }
    template < typename T >
    bool GetSerializedValue( ::hla::T_SerializerPtr serializer, T& val )
    {
        std::vector< uint8_t > buffer( serializer->GetSize() );
        if( !buffer.empty() )
            serializer->CopyTo( &buffer[0] );
        ::hla::Deserializer deserializer( &buffer[0], buffer.size() );
        val.Deserialize( deserializer );
        return true;
    }
}

#endif // hla_plugin_test_DeserializationTools_h
