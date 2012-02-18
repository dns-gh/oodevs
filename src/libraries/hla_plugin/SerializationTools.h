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
#include <hla/Serializer.h>
#include <hla/Deserializer.h>
#include <vector>

namespace hla
{
    template< unsigned N >
    Serializer& operator<<( Serializer& serializer, const unsigned char (&buffer)[N] )
    {
        for( unsigned i = 0; i < N; ++i )
            serializer << buffer[i];
        return serializer;
    }
    template< unsigned N >
    Deserializer& operator>>( Deserializer& deserializer, unsigned char (&buffer)[N] )
    {
        for( unsigned i = 0; i < N; ++i )
            deserializer >> buffer[i];
        return deserializer;
    }
    template< typename T >
    Serializer& operator<<( Serializer& serializer, const std::vector< T >& values )
    {
        for( std::vector< T >::const_iterator it = values.begin(); it != values.end(); ++it )
            serializer << *it;
        return serializer;
    }
    template< typename T >
    Deserializer& operator>>( Deserializer& deserializer, std::vector< T >& values )
    {
        for( std::vector< T >::iterator it = values.begin(); it != values.end(); ++it )
            deserializer >> *it;
        return deserializer;
    }
    template< typename T >
    Serializer& operator<<( Serializer& serializer, const T& parameter )
    {
        parameter.Serialize( serializer );
        return serializer;
    }
    template< typename T >
    Deserializer& operator>>( Deserializer& deserializer, T& parameter )
    {
        parameter.Deserialize( deserializer );
        return deserializer;
    }
}

#endif // plugins_hla_SerializationTools_h
