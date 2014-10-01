// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Chunker_ABC.h"

#include "Reply_ABC.h"
#include "runtime/Io.h"

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

using namespace web;

#ifdef _MSC_VER
#include <stdarg.h>
namespace
{
int vsnprintf( char* str, size_t size, const char* format, va_list list )
{
    if( size == 0 )
        return 0;
    if( size > INT_MAX )
        return -1;
    memset( str, 0, size );
    int rpy = _vsnprintf( str, size - 1, format, list );
    if( rpy == -1 )
        rpy = static_cast< int >( size );
    return rpy;
}

int snprintf( char* str, size_t size, const char* format, ...)
{
    va_list list;
    va_start( list, format );
    int rpy = vsnprintf( str, size, format, list );
    va_end( list );
    return rpy;
}
}
#endif

namespace
{
const size_t header_size = 64;
const size_t buffer_size = 1<<18;

// -----------------------------------------------------------------------------
// Name: Chunker
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
struct Chunker : public Chunker_ABC, public io::Writer_ABC
{
    // -----------------------------------------------------------------------------
    // Name: Chunker::Chunker
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    Chunker( Reply_ABC& rpy )
        : rpy_   ( rpy )
        , buffer_( header_size*2 + buffer_size )
        , fill_  ( 0 )
    {
        rpy_.SetStatus( web::OK );
        rpy_.SetHeader( "Transfer-Encoding", "chunked" );
        rpy_.SetHeader( "Connection", "Close" );
    }

    // -----------------------------------------------------------------------------
    // Name: Chunker::~Chunker
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    ~Chunker()
    {
        Flush( true );
    }

    // -----------------------------------------------------------------------------
    // Name: Chunker::SetHeader
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual void SetHeader( const std::string& key, const std::string& value )
    {
        rpy_.SetHeader( key, value );
    }

    // -----------------------------------------------------------------------------
    // Name: Chunker::SetName
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual void SetName( const std::string& name )
    {
        SetHeader( "Content-Disposition", "attachment; filename=\"" + name + "\"" );
    }

    // -----------------------------------------------------------------------------
    // Name: Chunker::OpenWriter
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual io::Writer_ABC& OpenWriter()
    {
        rpy_.WriteHeaders();
        return *this;
    }

    // -----------------------------------------------------------------------------
    // Name: Chunker::Flush
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    bool Flush( bool last )
    {
        const int header = snprintf( &buffer_[0], header_size, "%x\r\n", fill_ );
        memmove( &buffer_[header_size - header], &buffer_[0], header );
        const char* fmt  = last && fill_ ? "\r\n0\r\n\r\n" : "\r\n";
        const int footer = snprintf( &buffer_[header_size + fill_], header_size, fmt );
        const size_t next = header + fill_ + footer;
        const int len = rpy_.Write( &buffer_[header_size - header], next );
        if( len != static_cast< int >( next ) )
            return false;
        fill_ = 0;
        return true;
    }

    // -----------------------------------------------------------------------------
    // Name: Chunker::Write
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    int Write( const void* data, size_t size )
    {
        const char* ptr = reinterpret_cast< const char* >( data );
        size_t left = size;
        while( left )
        {
            const size_t step = std::min( left, buffer_size - fill_ );
            if( !step )
            {
                const bool valid = Flush( false );
                if( !valid )
                    return -1;
                continue;
            }

            memcpy( &buffer_[header_size + fill_], ptr, step );
            ptr   += step;
            fill_ += step;
            left  -= step;
        }
        return static_cast< int >( size );
    }

    Reply_ABC& rpy_;
    std::vector< char > buffer_;
    size_t fill_;
};
}

// -----------------------------------------------------------------------------
// Name: web::MakeChunker
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
boost::shared_ptr< Chunker_ABC > web::MakeChunker( Reply_ABC& rpy )
{
    return boost::make_shared< Chunker >( boost::ref( rpy ) );
}
