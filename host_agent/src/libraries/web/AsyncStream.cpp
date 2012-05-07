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

template< typename T >
struct Device : public boost::iostreams::source
{
    Device( T* ref ) : ref_( ref )
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
    T* ref_;
};
}

struct AsyncStream::Private : public boost::noncopyable
{
    Private()
        : reading_( false )
        , writing_( true )
    {
        // NOTHING
    }

    ~Private()
    {
        Join();
    }

    void Write( const char* data, size_t size )
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        if( !writing_ && !reading_ )
            return;
        buffer_.insert( buffer_.end(), data, data + size );
        condition_.notify_all();
    }

    void CloseWrite()
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        writing_ = false;
        condition_.notify_all();
    }

    void Read( AsyncStream::Handler handler )
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        reading_ = true;
        lock.unlock();

        Device< Private > device( this );
        boost::iostreams::stream< Device< Private > > stream( device );
        handler( stream );

        lock.lock();
        reading_ = false;
        condition_.notify_all();
    }

    void Join()
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        while( reading_ )
            condition_.wait( lock );
    }

    std::streamsize Read( char* data, std::streamsize size )
    {
        std::streamsize fill = 0;
        boost::unique_lock< boost::mutex > lock( access_ );
        while( fill < size )
        {
            // we will never get more data if producer is stopped and buffer is empty
            if( buffer_.empty() )
                if( !writing_ )
                    return fill ? fill : -1;
                else
                    condition_.wait( lock );
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
    bool reading_;
    bool writing_;
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
    private_->Write( data, size );
}

// -----------------------------------------------------------------------------
// Name: AsyncStream::CloseWrite
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
void AsyncStream::CloseWrite()
{
    private_->CloseWrite();
}

// -----------------------------------------------------------------------------
// Name: AsyncStream::Read
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
void AsyncStream::Read( Handler handler )
{
    private_->Read( handler );
}
