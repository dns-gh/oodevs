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
Message::Message( std::size_t size /*= 0*/ )
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
Message& Message::operator >>( unsigned long& n )
{
    Read( n );
    n = ntohl( n );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Message::Size
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
std::size_t Message::Size() const
{
    return data_->size() - readOffset_;
}

// -----------------------------------------------------------------------------
// Name: Message::Read
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Message::Read( char* data, std::size_t size )
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
void Message::Write( const char* data, std::size_t size )
{
    if( ! data_ )
        data_.reset( new T_Data( 2 * sizeof( unsigned long ) ) );
    std::copy( data, data + size, std::back_inserter( *data_ ) );
}

BOOST_STATIC_ASSERT( sizeof( unsigned long ) == 4 );

// -----------------------------------------------------------------------------
// Name: Message::MakeOutputBuffer
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
boost::asio::const_buffers_1 Message::MakeOutputBuffer( unsigned long tag ) const
{
    if( ! data_ || data_->size() < 2 * sizeof( unsigned long ) )
        return boost::asio::const_buffers_1( boost::asio::const_buffer() );
    unsigned long* data = reinterpret_cast< unsigned long* >( &data_->front() );
    std::size_t size = data_->size() - sizeof( unsigned long );
    *data = htonl( (unsigned long)size );
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
