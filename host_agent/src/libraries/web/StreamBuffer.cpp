// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#define __STDC_LIMIT_MACROS
#include "StreamBuffer.h"

#include "runtime/Io.h"

#include <cstring>
#include <stdexcept>
#include <stdint.h>

using namespace web;

// -----------------------------------------------------------------------------
// Name: StreamBuffer::StreamBuffer
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
StreamBuffer::StreamBuffer( io::Reader_ABC& src )
    : src_   ( src )
    , skip_  ( 0 )
    , size_  ( 0 )
    , buffer_( UINT16_MAX )
    , eof_   ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::~StreamBuffer
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
StreamBuffer::~StreamBuffer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::Eof
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
bool StreamBuffer::Eof()
{
    FillAtLeast( 1 );
    return !size_ && eof_;
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::Peek
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
char StreamBuffer::Peek()
{
    FillAtLeast( 64 );
    if( !size_ )
        throw std::runtime_error( "buffer overread" );
    return buffer_[skip_];
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::Get
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
char StreamBuffer::Get()
{
    const char reply = Peek();
    Skip( 1 );
    return reply;
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::Peek
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
size_t StreamBuffer::Peek( char** dst )
{
    FillAtLeast( INT16_MAX );
    *dst = &buffer_[skip_];
    return size_;
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::Skip
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
void StreamBuffer::Skip( size_t offset )
{
    FillAtLeast( offset );
    SkipLocally( offset );
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::SkipLocally
// Created: BAX 2012-05-07
// -----------------------------------------------------------------------------
void StreamBuffer::SkipLocally( size_t offset )
{
    offset = std::min( offset, size_ );
    skip_ += offset;
    size_ -= offset;
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::Read
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
size_t StreamBuffer::Read( char* dst, size_t size )
{
    FillAtLeast( size );
    const size_t next = std::min( size_, size );
    memcpy( dst, &buffer_[skip_], next );
    SkipLocally( next );
    return next;
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::Read
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
void StreamBuffer::FillAtLeast( size_t size )
{
    if( size_ >= size )
        return;
    if( eof_ )
        return;
    if( skip_ )
        memmove( &buffer_[0], &buffer_[skip_], size_ );
    skip_ = 0;
    const int len = src_.Read( &buffer_[size_], buffer_.size() - size_ );
    eof_ |= len <= 0;
    if( eof_ )
        return;
    size_ += len;
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::GetLine
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
std::string StreamBuffer::GetLine()
{
    std::string reply;
    for(;;)
    {
        FillAtLeast( 256 );
        if( !size_ )
            break;
        const char* src  = &buffer_[skip_];
        const void* next = memchr( src, '\n', size_ );
        const char* ptr  = next ? static_cast< const char* >( next ) : &src[size_];
        const ptrdiff_t diff = ptr - src;
        reply.append( src, diff );
        SkipLocally( diff + 1 );
        if( next )
            break;
    }
    return reply;
}
