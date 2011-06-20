// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_test_Tools_h
#define hla_plugin_test_Tools_h

#include <vector>
#include <hla/Serializer.h>
#include <hla/Deserializer.h>
#include <boost/shared_ptr.hpp>

namespace
{
    typedef std::vector< int8 > T_Buffer;
    class SerializationFixture
    {
    public:
        template< typename T >
        ::hla::Deserializer Serialize( const T& value, unsigned int messageSize )
        {
            ::hla::Serializer serializer;
            value.Serialize( serializer );
            boost::shared_ptr< T_Buffer > buffer( new T_Buffer( serializer.GetSize() ) );
            if( !buffer->empty() )
                serializer.CopyTo( &(*buffer)[0] );
            BOOST_CHECK_EQUAL( messageSize, buffer->size() );
            ::hla::Deserializer deserializer( &(*buffer)[0], buffer->size() );
            buffers_.push_back( buffer );
            return deserializer;
        }
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
                Read< int8 >( deserializer );
        }
        template< unsigned N >
        void ReadPadding( ::hla::Deserializer& deserializer )
        {
            for( unsigned int i = 0; i < N; ++i )
                BOOST_CHECK_EQUAL( 0, Read< int8 >( deserializer ) );
        }
    private:
        std::vector< boost::shared_ptr< T_Buffer > > buffers_;
    };
}

#endif // hla_plugin_test_Tools_h
