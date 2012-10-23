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

#include "DeserializationTools.h"
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
        ::hla::Deserializer Serialize( const T& value, size_t messageSize )
        {
            ::hla::Serializer serializer;
            value.Serialize( static_cast< ::hla::Serializer_ABC& >( serializer ) );
            boost::shared_ptr< T_Buffer > buffer( new T_Buffer( serializer.GetSize() ) );
            if( !buffer->empty() )
                serializer.CopyTo( &(*buffer)[0] );
            BOOST_CHECK_EQUAL( messageSize, buffer->size() );
            ::hla::Deserializer deserializer( &(*buffer)[0], buffer->size() );
            buffers_.push_back( buffer );
            return deserializer;
        }
        template< typename T >
        ::hla::Deserializer Serialize( const T& value )
        {
            ::hla::Serializer serializer;
            value.Serialize( static_cast< ::hla::Serializer_ABC& >( serializer ) );
            boost::shared_ptr< T_Buffer > buffer( new T_Buffer( serializer.GetSize() ) );
            if( !buffer->empty() )
                serializer.CopyTo( &(*buffer)[0] );
            ::hla::Deserializer deserializer( &(*buffer)[0], buffer->size() );
            buffers_.push_back( buffer );
            return deserializer;
        }
    private:
        std::vector< boost::shared_ptr< T_Buffer > > buffers_;
    };
}

#endif // hla_plugin_test_Tools_h
