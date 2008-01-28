// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "Message.h"
#include "asio.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Message constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message::Message()
    : readOffset_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Message constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message::Message( unsigned long size /*= 0*/ )
    : readOffset_( 0 )
    , data_      ( new T_Data( size ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Message destructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message::~Message()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( bool& n )
{
    char value;
    Read( value );
    n = value != 0;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( unsigned char& n )
{
    Read( n );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( char& n )
{
    Read( n );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( unsigned short& n )
{
    Read( n );
    n = ntohs( n );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( short& n )
{
    Read( n );
    n = ntohs( n );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( unsigned long& n )
{
    Read( n );
    n = ntohl( n );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( long& n )
{
    Read( n );
    n = ntohl( n );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( unsigned long long& n )
{
    unsigned long* p = reinterpret_cast< unsigned long* >( &n );
    return *this >> *(p+1) >> *p;
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( long long& n )
{
    long* p = reinterpret_cast< long* >( &n );
    return *this >> *(p+1) >> *p;
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( float& n )
{
    return operator>>( *reinterpret_cast< long* >( &n ) );
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( double& n )
{
    return operator>>( *reinterpret_cast< long long* >( &n ) );
}

// -----------------------------------------------------------------------------
// Name: Message::operator >>
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator >>( std::string& string )
{
    unsigned short size;
    operator>>( size );
    string.resize( size );
    Read( &string[0], size );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( bool n )
{
    char value = n ? 1 : 0;
    return operator<<( value );
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( unsigned char n )
{
    Write( n );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( char n )
{
    Write( n );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( unsigned short n )
{
    Write( htons( n ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( short n )
{
    Write( htons( n ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( unsigned long n )
{
    Write( htonl( n ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( long n )
{
    Write( htonl( n ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( unsigned long long n )
{
    const unsigned long* p = reinterpret_cast< const unsigned long* >( &n );
    return *this << *(p+1) << *p;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( long long n )
{
    const long* p = reinterpret_cast< const long* >( &n );
    return *this << *(p+1) << *p;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( float n )
{
    return operator<<( *reinterpret_cast< const long* >( &n ) );
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( double n )
{
    return operator<<( *reinterpret_cast< const long long* >( &n ) );
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( const std::string& string )
{
    unsigned short size = unsigned short( string.size() );
    operator<<( size );
    Write( string.c_str(), size );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::operator <<
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Message& Message::operator <<( const char* pString )
{
    return operator<<( std::string( pString ) );
}

// -----------------------------------------------------------------------------
// Name: Message::Size
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
unsigned long Message::Size() const
{
    return data_->size() - readOffset_;
}

// -----------------------------------------------------------------------------
// Name: Message::Read
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Message::Read( char* data, unsigned size )
{
    if( ! size )
        return;
    if( ! data_ || readOffset_ + size > data_->size() )
        throw std::overflow_error( "Message read overflow" );
    memcpy( data, & (*data_)[ readOffset_ ], size );
    readOffset_ += size;
}

// -----------------------------------------------------------------------------
// Name: Message::Write
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Message::Write( const char* data, unsigned size )
{
    if( ! data_ )
        data_.reset( new T_Data( 2 * sizeof( unsigned long ) ) );
    std::copy( data, data + size, std::back_inserter( *data_ ) );
}

// -----------------------------------------------------------------------------
// Name: Message::MakeOutputBuffer
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
boost::asio::const_buffers_1 Message::MakeOutputBuffer( unsigned long tag ) const
{
    if( ! data_ || data_->size() < 8 )
        return boost::asio::const_buffers_1( boost::asio::const_buffer() );
    unsigned long* data = reinterpret_cast< unsigned long* >( & data_->front() );
    *data     = htonl( data_->size() - 4 );
    *(data+1) = htonl( tag );
    const T_Data& buffer = *data_;
    return boost::asio::buffer( buffer );
}

// -----------------------------------------------------------------------------
// Name: Message::MakeInputBuffer
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
boost::asio::mutable_buffers_1 Message::MakeInputBuffer()
{
    return boost::asio::buffer( *data_ );
}
