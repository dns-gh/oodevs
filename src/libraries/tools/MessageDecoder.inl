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
// Name: MessageDecoder constructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T >
MessageDecoder< T >::MessageDecoder( Message& message )
{
    const unsigned long size = message.Size();
    // $$$$ SBO 2009-10-26: Find a way to get rid of new
    char* buffer = new char[ size ];
    message.Read( buffer, size );
    if( ! message_.ParseFromString( std::string( buffer, size ) ) )
        throw std::runtime_error( __FUNCTION__ ": message deserialization failed." );
    delete[] buffer;
}

// -----------------------------------------------------------------------------
// Name: MessageDecoder::GetAsnMsg
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T >
MessageDecoder< T >::operator const T& () const
{
    return message_;
}

}