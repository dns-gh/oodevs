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
        for( typename std::vector< T >::const_iterator it = values.begin(); it != values.end(); ++it )
            serializer << *it;
        return serializer;
    }
    template< typename T >
    Deserializer_ABC& operator>>( Deserializer_ABC& deserializer, std::vector< T >& values )
    {
        if( values.size() != 0 )
        {
            for( typename std::vector< T >::iterator it = values.begin(); it != values.end(); ++it )
                deserializer >> *it;
        }
        else
        {
            while( deserializer.GetSize() != 0 )
            {
                T tmp;
                deserializer >> tmp;
                values.push_back( tmp );
            }
        }
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
            int32_t size = static_cast< int32_t >( list.size() );
            archive << size
                    << list;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            int32_t size = 0;
            archive >> size;
            list.resize( size );
            archive >> list;
        }
        std::vector< ElemType > list;
    };
    // ALIGN : alignment in bytes
    template < typename T, uint32_t ALIGN=4 >
    struct PaddedVariableArray
    {
        typedef T ElemType;
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            int32_t size = static_cast< int32_t >( list.size() );
            archive << size;
            if( size == 0 )
                return;
            writePadding( archive, 4 );
            for( int32_t i = 0; i < size; ++i)
            {
                unsigned long st = archive.GetSize();
                archive << list[i];
                unsigned long en = archive.GetSize();
                if( i < size-1 )
                    writePadding( archive, en-st );
            }
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            int32_t size = 0;
            archive >> size;
            list.resize( size );
            if( size == 0 )
                return;
            readPadding( archive, 4 );
            for( int32_t i = 0; i < size; ++i)
            {
                T tmp;
                std::size_t st = archive.GetSize();
                archive >> tmp;
                list[i] = tmp;
                std::size_t en = archive.GetSize();
                // since we are deserializing, st > en
                if( i < size-1 )
                    readPadding( archive, st-en );
            }
        }
        std::vector< ElemType > list;

    private:
        template< typename Archive >
        void writePadding( Archive& archive, unsigned long diff ) const
        {
            unsigned long padd = diff % ALIGN != 0 ? ALIGN - ( diff % ALIGN ): 0;
            for( ; padd > 0; --padd )
                archive << (uint8_t)0;
        }
        template< typename Archive >
        void readPadding( Archive& archive, std::size_t diff )
        {
            uint8_t garb;
            unsigned long padd = diff % ALIGN != 0 ? ALIGN - ( diff % ALIGN ): 0;
            for( ; padd > 0; --padd )
                archive >> garb;
        }
    };
}
}

#endif // plugins_hla_SerializationTools_h
