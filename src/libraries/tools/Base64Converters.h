// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __Base64Converters_h_
#define __Base64Converters_h_

namespace tools
{
    // standard conversion
    std::string BinaryToBase64( const std::string& binary );
    std::string Base64ToBinary( const std::string& base64 );

    // protobuf binary conversion
    template< typename T >
    std::string ProtoToBinary( const T& message )
    {
        std::stringstream binaryStream;
        message.SerializeToOstream( &binaryStream );
        return binaryStream.str();
    }
    template< typename T >
    T BinaryToProto( const std::string& binary )
    {
        std::stringstream binaryStream;
        binaryStream << binary;
        T message;
        message.ParseFromIstream( &binaryStream );
        return message;
    }

    // protobuf base64 conversion
    template< typename T >
    std::string ProtoToBase64( const T& message )
    {
        return BinaryToBase64( ProtoToBinary( message ) );
    }
    template< typename T >
    T Base64ToProto( const std::string& base64 )
    {
        return BinaryToProto< T >( Base64ToBinary( base64 ) );
    }
}

#endif // __Base64Converters_h_
