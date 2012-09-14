// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Io.h"

#include <boost/make_shared.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

using namespace io;

namespace
{
struct Buffer
{
    Buffer()
        : data_( 0 )
        , size_( 0 )
    {
        // NOTHING
    }

    ~Buffer()
    {
        // NOTHING
    }

    void Store( const char* src, size_t size )
    {
        assert( !size_ );
        data_ = src;
        size_ = size;
    }

    size_t Consume( char* dst, size_t size )
    {
        assert( size_ );
        const size_t next = std::min( size, size_ );
        memcpy( dst, data_, next );
        size_ -= next;
        data_ += next;
        return next;
    }

    bool IsEmpty() const
    {
        return !size_;
    }

    size_t Skip()
    {
        const size_t left = size_;
        size_ = 0;
        return left;
    }

    private:
    const char* data_;
    size_t size_;
};

struct Pipe : public Pipe_ABC
{
    Pipe()
        : eof_( false )
    {
        // NOTHING
    }

    ~Pipe()
    {
        // NOTHING
    }

    size_t Write( const void* ptr, size_t size )
    {
        const char* src = reinterpret_cast< const char* >( ptr );
        boost::unique_lock< boost::mutex > lock( access_ );
        buf_.Store( src, size );
        read_.notify_one();
        while( !eof_ && !buf_.IsEmpty() )
            write_.wait( lock );
        return size - buf_.Skip();
    }

    size_t Read( void* ptr, size_t size )
    {
        char* dst = reinterpret_cast< char* >( ptr );
        boost::unique_lock< boost::mutex > lock( access_ );
        while( !eof_ && buf_.IsEmpty() )
            read_.wait( lock );
        if( eof_ )
            return 0;
        const size_t read = buf_.Consume( dst, size );
        if( buf_.IsEmpty() )
            write_.notify_one();
        return read;
    }

    void Close()
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        eof_ = true;
        write_.notify_one();
        read_.notify_one();
    }

private:
    boost::mutex access_;
    boost::condition_variable read_;
    boost::condition_variable write_;
    Buffer buf_;
    bool eof_;
};
}

boost::shared_ptr< Pipe_ABC > io::MakePipe()
{
    return boost::make_shared< Pipe >();
}
