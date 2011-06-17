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

namespace
{
    typedef std::vector< int8 > T_Buffer;
    T_Buffer Convert( const ::hla::Serializer& serializer )
    {
        T_Buffer buffer( serializer.GetSize() );
        if( !buffer.empty() )
            serializer.CopyTo( &buffer[0] );
        return buffer;
    }
    template< typename T >
    T Read( ::hla::Deserializer& deserializer )
    {
        T result;
        deserializer >> result;
        return result;
    }
}

#endif // hla_plugin_test_Tools_h
