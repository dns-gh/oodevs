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
#include "hla_plugin/Omt13String.h"
#include "hla_plugin/SerializationTools.h"
#include "hla_plugin/UniqueId.h"
#include "hla_plugin/UnicodeString.h"
#include "hla_plugin/InteractionsNetn.h"
#include <vector>
#include <hla/Serializer.h>
#include <hla/Deserializer.h>
#include <hla/Interaction_ABC.h>
#include <boost/shared_ptr.hpp>

namespace plugins
{
namespace hla
{
    inline bool operator == (const Omt13String& lhs, const Omt13String& rhs)
    {
        return lhs.str() == rhs.str();
    }

    inline bool operator==( const UnicodeString& lhs, const UnicodeString& rhs )
    {
        return lhs.str() == rhs.str();
    }
    inline bool operator==( const NETN_UUID& lhs, const NETN_UUID& rhs )
    {
        return lhs.str() == rhs.str();
    }

    template < typename T >
    inline bool operator==( const VariableArray< T >& lhs, const VariableArray< T >& rhs )
    {
        return lhs.list == rhs.list;
    }
}
}

namespace
{
    typedef std::vector< uint8_t > T_Buffer;
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

    template <typename T>
    void FillParameter( std::list< std::vector<uint8_t> >& bufVect, ::hla::Interaction_ABC::T_Parameters& p,
            const std::string& paramName, const T& value )
    {
        ::hla::Serializer ser;
        ser << value;
        std::vector<uint8_t> buff(ser.GetSize(), 0 );
        ser.CopyTo( &buff[0] );
        bufVect.push_back( buff );
        p.push_back( std::make_pair( ::hla::ParameterIdentifier( paramName ), new ::hla::Deserializer( &(bufVect.back()[0]), ser.GetSize() ) ) );
    }
    template <typename T>
    void CheckParameter( const ::hla::ParameterIdentifier& param, ::hla::T_SerializerPtr serializer, const T& ref )
    {
        T value;
        std::vector<uint8_t> buff( serializer->GetSize(), 0 );
        serializer->CopyTo( &buff[0] );
        ::hla::Deserializer deser( &buff[0], serializer->GetSize() );
        deser >> value;
        BOOST_CHECK_MESSAGE( value == ref, std::string("Failed verification on value of parameter ") + param.ToString() );
    }
}

#endif // hla_plugin_test_Tools_h
