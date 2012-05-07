// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "AsyncStream.h"

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4702 )
#endif
#include <boost/iostreams/stream.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

#include <boost/thread/condition_variable.hpp>
#include <vector>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace web;

namespace
{
template< typename T >
void MoveData( T* dst, std::vector< T >& src, std::streamsize size )
{
    std::copy( &src[0], &src[0] + size, dst );
    src.erase( src.begin(), src.begin() + static_cast< size_t >( size ) );
}

struct Buffer : public boost::noncopyable
{
    Buffer()
        : running_( false )
        , stopped_( false )
    {
        // NOTHING
    }

    ~Buffer()
    {
        // NOTHING
    }

    void Write( const char* data, size_t size )
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        running_ = true;
        buffer_.insert( buffer_.end(), data, data + size );
        condition_.notify_all();
    }

    void Close()
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        stopped_ = true;
        condition_.notify_all();
    }

    void Join()
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        while( running_ )
            condition_.wait( lock );
    }

    std::streamsize Read( char* data, std::streamsize size )
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        if( stopped_ && buffer_.empty() )
        {
            running_ = false;
            condition_.notify_all();
            return -1;
        }

        std::streamsize fill = 0;
        while( fill < size )
        {
            // we will never get more data if producer is stopped and buffer is empty
            if( !stopped_ )
                condition_.wait( lock );
            else if( buffer_.empty() )
                break;
            std::streamsize next = std::min( size - fill, static_cast< std::streamsize >( buffer_.size() ) );
            if( next )
                MoveData( data + fill, buffer_, next );
            fill += next;
        }
        return fill;
    }

private:
    boost::mutex access_;
    boost::condition_variable condition_;
    std::vector< char > buffer_;
    bool running_;
    bool stopped_;
};

struct Device : public boost::iostreams::source
{
    Device( Buffer* ref ) : ref_( ref )
    {
        // NOTHING
    }

    ~Device()
    {
        // NOTHING
    }

    std::streamsize read( char* data, std::streamsize size )
    {
        return ref_->Read( data, size );
    }

private:
    Buffer* ref_;
};
}

struct AsyncStream::Private
{
    Private()
        : device_( &buffer_ )
        , stream_( device_ )
    {
        // NOTHING
    }

    ~Private()
    {
        buffer_.Join();
    }

    Buffer buffer_;
    Device device_;
    boost::iostreams::stream< Device > stream_;
};

// -----------------------------------------------------------------------------
// Name: AsyncStream::AsyncStream
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
AsyncStream::AsyncStream()
    : private_( new Private() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AsyncStream::~AsyncStream
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
AsyncStream::~AsyncStream()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AsyncStream::Write
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
void AsyncStream::Write( const char* data, size_t size )
{
    private_->buffer_.Write( data, size );
}

// -----------------------------------------------------------------------------
// Name: AsyncStream::Close
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
void AsyncStream::Close()
{
    private_->buffer_.Close();
}

// -----------------------------------------------------------------------------
// Name: AsyncStream::Get
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
std::istream& AsyncStream::Get()
{
    return private_->stream_;
}
