// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace tools {

// -----------------------------------------------------------------------------
// Name: MessageEncoder constructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T >
MessageEncoder< T >::MessageEncoder( const T& message )
{
    if( !message.IsInitialized() )
    {
        std::stringstream ss;
        ss  << "Message of type \"" << message.GetDescriptor()->full_name()
            << "\" is missing required fields: " << message.InitializationErrorString();
        throw std::runtime_error( ss.str() );
    }

    // $$$$ SEB 2009-10-27: Alternate implementation, little bit slower but uses stack instead of heap -> less fragmentation
//    std::string buffer;
//    if( !message.SerializeToString( &buffer ) )
//    {
//        std::stringstream ss;
//        ss << "Error serializing message of type \"" << message.GetDescriptor()->full_name() << "\"" << std::endl;
//        throw std::runtime_error( ss.str() );
//    }
//    message_.Write( buffer.c_str(), buffer.size() );

    google::protobuf::uint8* buffer = new google::protobuf::uint8[ message.ByteSize() ];
    if( !message.SerializeWithCachedSizesToArray( buffer ) )
    {
        std::stringstream ss;
        ss << "Error serializing message of type \"" << message.GetDescriptor()->full_name() << "\"" << std::endl;
        throw std::runtime_error( ss.str() );
    }
    message_.Write( (const char*)buffer, message.GetCachedSize() );
    delete[] buffer;
}

// -----------------------------------------------------------------------------
// Name: MessageEncoder::GetDinMsg
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T >
MessageEncoder< T >::operator const Message& () const
{
    return message_;
}

}