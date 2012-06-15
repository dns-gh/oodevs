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

#include <boost/function.hpp>
#include <boost/optional.hpp>
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
    if( !size )
        return;
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

struct Scoper
{
    typedef boost::function< void( void ) > Task;
    Scoper( const Task& task ) : task_( task )
    {
        // NOTHING
    }
    ~Scoper()
    {
        task_();
    }
private:
    Task task_;
};
}

struct AsyncStream::Private : public boost::noncopyable
{
    Private()
        : capacity_( UINT16_MAX )
        , writing_ ( true )
        , reading_ ( boost::none )
    {
        buffer_.reserve( capacity_ );
    }

    ~Private()
    {
        Join();
    }

    void WakeAll()
    {
        condition_.notify_all();
    }

    template< typename T >
    void SetState( T& dst, bool value )
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        dst = value;
        WakeAll();
    }

    bool ShouldWrite() const
    {
        return writing_ && ( reading_ == boost::none || *reading_ );
    }

    bool HasCapacity() const
    {
        return buffer_.size() < capacity_;
    }

    void Write( const char* data, size_t size )
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        for( size_t fill = 0; fill < size; )
        {
            condition_.wait( lock, !boost::bind( &Private::ShouldWrite, this ) || boost::bind( &Private::HasCapacity, this ) );
            if( !ShouldWrite() )
                return;
            const size_t next = std::min( size - fill, capacity_ - buffer_.size() );
            buffer_.insert( buffer_.end(), data + fill, data + fill + next );
            fill += next;
            condition_.notify_all();
        }
    }

    void CloseWrite()
    {
        SetState( writing_, false );
    }

    void Read( AsyncStream::Handler handler )
    {
        SetState( reading_, true );
        Scoper stop( boost::bind( &Private::SetState< boost::optional< bool > >, this, boost::ref( reading_ ), false ) );
        Device< Private > device( this );
        boost::iostreams::stream< Device< Private > > stream( device );
        handler( stream );
    }

    bool IsReading() const
    {
        return reading_ != boost::none && *reading_;
    }

    void Join()
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        condition_.wait( lock, !boost::bind( &Private::IsReading, this ) );
    }

    bool IsWriteStoppedOrHasData() const
    {
        return !writing_ || buffer_.size();
    }

    std::streamsize Read( char* data, std::streamsize size )
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        for( std::streamsize fill = 0; fill < size; )
        {
            Scoper wake( boost::bind( &Private::WakeAll, this ) );
            condition_.wait( lock, boost::bind( &Private::IsWriteStoppedOrHasData, this ) );
            const std::streamsize next = std::min( size - fill, static_cast< std::streamsize >( buffer_.size() ) );
            if( !next && !writing_ )
                return fill ? fill : -1;
            MoveData( data + fill, buffer_, next );
            fill += next;
            condition_.notify_all();
        }
        return size;
    }

private:
    boost::mutex access_;
    const size_t capacity_;
    std::vector< char > buffer_;
    boost::condition_variable condition_;
    boost::optional< bool > reading_;
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
