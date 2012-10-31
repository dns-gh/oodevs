// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_SerializationTools_h
#define plugins_hla_SerializationTools_h

#include <hla/Encodable.h>
#include <hla/Serializer_ABC.h>
#include <hla/Deserializer_ABC.h>
#include <vector>

namespace hla
{
    template< unsigned N >
    Serializer_ABC& operator<<( Serializer_ABC& serializer, const unsigned char (&buffer)[N] )
    {
        for( unsigned i = 0; i < N; ++i )
            serializer << buffer[i];
        return serializer;
    }
    template< unsigned N >
    Deserializer_ABC& operator>>( Deserializer_ABC& deserializer, unsigned char (&buffer)[N] )
    {
        for( unsigned i = 0; i < N; ++i )
            deserializer >> buffer[i];
        return deserializer;
    }
    template< typename T >
    Serializer_ABC& operator<<( Serializer_ABC& serializer, const std::vector< T >& values )
    {
        for( std::vector< T >::const_iterator it = values.begin(); it != values.end(); ++it )
            serializer << *it;
        return serializer;
    }
    template< typename T >
    Deserializer_ABC& operator>>( Deserializer_ABC& deserializer, std::vector< T >& values )
    {
        for( std::vector< T >::iterator it = values.begin(); it != values.end(); ++it )
            deserializer >> *it;
        return deserializer;
    }
    template< typename T >
    Serializer_ABC& operator<<( Serializer_ABC& serializer, const T& parameter )
    {
        parameter.Serialize( serializer );
        return serializer;
    }
    template< typename T >
    Deserializer_ABC& operator>>( Deserializer_ABC& deserializer, T& parameter )
    {
        parameter.Deserialize( deserializer );
        return deserializer;
    }
}

namespace plugins
{
namespace hla
{
    template < typename T >
    struct VariableArray
    {
        typedef T ElemType;
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            int32 size = static_cast< int32 >( list.size() );
            archive << size
                    << list;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            int32 size = 0;
            archive >> size;
            list.resize( size );
            archive >> list;
        }
        std::vector< ElemType > list;
    };
}
}

#endif // plugins_hla_SerializationTools_h
