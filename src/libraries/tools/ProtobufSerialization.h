// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ProtobufSerialization_h_
#define __ProtobufSerialization_h_

namespace tools
{
    template< typename T >
    std::string ProtoToBinary( const T& message )
    {
        std::stringstream binaryStream;
        message.SerializePartialToOstream( &binaryStream );
        return binaryStream.str();
    }
    template< typename T >
    T BinaryToProto( const std::string& binary )
    {
        std::stringstream binaryStream;
        binaryStream << binary;
        T message;
        message.ParsePartialFromIstream( &binaryStream );
        return message;
    }
}

#endif // __ProtobufSerialization_h_
