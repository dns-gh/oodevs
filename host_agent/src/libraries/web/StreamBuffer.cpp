// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "StreamBuffer.h"

using namespace web;


// -----------------------------------------------------------------------------
// Name: StreamBuffer::StreamBuffer
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
StreamBuffer::StreamBuffer( std::istream& src )
    : src_   ( src )
    , skip_  ( 0 )
    , size_  ( 0 )
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
bool StreamBuffer::Eof() const
{
    return !size_ && src_.eof();
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::Peek
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
char StreamBuffer::Peek()
{
    FillAtLeast( 64 );
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
    FillAtLeast( INT_MAX );
    *dst = &buffer_[skip_];
    return size_;
}

// -----------------------------------------------------------------------------
// Name: StreamBuffer::Skip
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
void StreamBuffer::Skip( size_t offset )
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
    Skip( next );
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
    if( src_.eof() )
        return;
    if( skip_ )
        memmove( &buffer_[0], &buffer_[skip_], size_ );
    skip_ = 0;
    if( src_.eof() )
        return;
    src_.read( &buffer_[size_], sizeof buffer_ - size_ );
    size_ += static_cast< size_t >( src_.gcount() );
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
        const char* src  = &buffer_[skip_];
        const void* next = memchr( src, '\n', size_ );
        const char* ptr  = next ? static_cast< const char* >( next ) : &src[size_];
        const ptrdiff_t diff = ptr - src;
        reply.append( src, diff );
        Skip( diff + 1 );
        if( next )
            break;
    }
    return reply;
}
