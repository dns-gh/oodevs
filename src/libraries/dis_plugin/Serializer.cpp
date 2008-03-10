// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "Serializer.h"
#include <winsock2.h>

using namespace dis;

// -----------------------------------------------------------------------------
// Name: Serializer destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Serializer::~Serializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Serializer::Write
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
template< typename T >
void Serializer::Write( const T& data, unsigned size )
{
    if( offset_ + size > size_ )
        throw std::runtime_error( __FUNCTION__ );
    ::memcpy( data_ + offset_, &data, size );
    offset_ += size;
}

// -----------------------------------------------------------------------------
// Name: Serializer::operator<<
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Serializer& Serializer::operator<<( const float& r )
{
    return *this << ( *reinterpret_cast< const long* >( &r ) );
}

// -----------------------------------------------------------------------------
// Name: Serializer::operator<<
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Serializer& Serializer::operator<<( const double& r )
{
    typedef unsigned long uint32;
    uint32 buff[2];
    *((uint32*)buff) = htonl(*(((uint32*)&r)+1));
    *(((uint32*)buff)+1) = htonl(*((uint32*)&r));
    return *this << buff[0] << buff[1];
}

// -----------------------------------------------------------------------------
// Name: Serializer::operator<<
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Serializer& Serializer::operator<<( const char& c )
{
    Write( c, sizeof( char ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Serializer::operator<<
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Serializer& Serializer::operator<<( const unsigned char& c )
{
    Write( c, sizeof( char ) );
    return *this;
}
 
// -----------------------------------------------------------------------------
// Name: Serializer::operator<<
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Serializer& Serializer::operator<<( const short& n )
{
    Write( htons( n ), sizeof( short ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Serializer::operator<<
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Serializer& Serializer::operator<<( const unsigned short& n )
{
    Write( htons( n ), sizeof( short ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Serializer::operator<<
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Serializer& Serializer::operator<<( const long& n )
{
    Write( htonl( n ), sizeof( long ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Serializer::operator<<
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Serializer& Serializer::operator<<( const unsigned long& n )
{
    Write( htonl( n ), sizeof( long ) );
    return *this;
}
