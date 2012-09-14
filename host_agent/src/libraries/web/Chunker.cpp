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
#include <boost/shared_ptr.hpp>

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
const size_t prolog_size = 64;
const size_t buffer_size = 1<<18;

// -----------------------------------------------------------------------------
// Name: Chunker
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
struct Chunker : public Chunker_ABC, public io::Writer_ABC
{
    Chunker( Reply_ABC& rpy )
        : rpy_   ( rpy )
        , buffer_( prolog_size*2 + buffer_size )
        , fill_  ( 0 )
    {
        rpy_.SetStatus( web::OK );
        rpy_.SetHeader( "Content-Type", "application/zip" );
        rpy_.SetHeader( "Transfer-Encoding", "chunked" );
        rpy_.SetHeader( "Connection", "Close" );
    }

    ~Chunker()
    {
        Flush( true );
    }

    io::Writer_ABC& SetName( const std::string& name )
    {
        rpy_.SetHeader( "Content-Disposition", "attachment; filename=\"" + name + ".zip\"" );
        rpy_.WriteHeaders();
        return *this;
    }

    void Flush( bool last )
    {
        const int prolog = snprintf( &buffer_[0], prolog_size, "%x\r\n", fill_ );
        memmove( &buffer_[prolog_size - prolog], &buffer_[0], prolog );
        const char* fmt  = last && fill_ ? "\r\n0\r\n\r\n" : "\r\n";
        const int epilog = snprintf( &buffer_[prolog_size + fill_], prolog_size, fmt );
        rpy_.Write( &buffer_[prolog_size - prolog], prolog + fill_ + epilog );
        fill_ = 0;
    }

    bool Write( const void* data, size_t size )
    {
        const char* ptr = reinterpret_cast< const char* >( data );
        while( size )
        {
            const size_t step = std::min( size, buffer_size - fill_ );
            if( !step )
            {
                Flush( false );
                continue;
            }

            memcpy( &buffer_[prolog_size + fill_], ptr, step );
            ptr   += step;
            fill_ += step;
            size  -= step;
        }
        return true;
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
    return boost::make_shared< Chunker >( rpy );
}
