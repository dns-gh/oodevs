// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#   pragma warning( disable : 4702 )
#endif

#include "Pool.h"

#include <boost/make_shared.hpp>

using namespace runtime;

namespace
{
struct TaskHandler
{
    typedef boost::packaged_task< void > Package;

    template< typename T >
    TaskHandler( Pool* pool, const T& task )
        : pool_   ( pool )
        , package_( boost::make_shared< Package >( task ) )
    {
        // NOTHING
    }

    void operator()() const
    {
        pool_->Acquire();
        (*package_)();
    }

    Pool* pool_;
    boost::shared_ptr< Package > package_;
};
}

// -----------------------------------------------------------------------------
// Name: Pool::Pool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
Pool::Pool( uint32_t cache, uint32_t max )
    : cache_( cache )
    , max_  ( max )
    , load_ ( 0 )
    , idle_ ( 0 )
    , work_ ( service_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Pool::~Pool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
Pool::~Pool()
{
    Stop();
}

// -----------------------------------------------------------------------------
// Name: Pool::AddThread
// Created: BAX 2013-01-07
// -----------------------------------------------------------------------------
void Pool::AddThread()
{
    boost::lock_guard< boost::mutex > lock( access_ );
    load_++;
    const size_t current = threads_.size();
    if( current >= max_ )
        return;
    if( load_ <= current )
        return;
    idle_++;
    threads_.push_back( T_Thread( new boost::thread( boost::bind( &Pool::RunThread, this ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: Pool::Acquire
// Created: BAX 2013-01-07
// -----------------------------------------------------------------------------
void Pool::Acquire()
{
    boost::lock_guard< boost::mutex > lock( access_ );
    idle_--;
}

namespace
{
bool IsThread( Pool::T_Thread thread, boost::thread::id id )
{
    return thread->get_id() == id;
}

void RemoveThread( Pool::T_Threads& dst, boost::thread::id id )
{
    auto it = std::find_if( dst.begin(), dst.end(), boost::bind( &IsThread, _1, id ) );
    if( it != dst.end() )
        dst.erase( it );
}
}

// -----------------------------------------------------------------------------
// Name: Pool::RunThread
// Created: BAX 2013-01-07
// -----------------------------------------------------------------------------
void Pool::RunThread()
{
    const auto id = boost::this_thread::get_id();
    while( service_.run_one() )
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        load_--;
        if( idle_ + 1 > cache_ )
            return RemoveThread( threads_, id );
        idle_++;
    }
}

// -----------------------------------------------------------------------------
// Name: Pool::Post
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
Pool::Future Pool::Post( const Task& task )
{
    AddThread();
    TaskHandler handler( this, task );
    service_.post( handler );
    return Future( handler.package_->get_future() );
}

// -----------------------------------------------------------------------------
// Name: Pool::Stop
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
void Pool::Stop()
{
    service_.stop();
    for( ;; )
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        if( threads_.empty() )
            break;
        const auto local = threads_;
        threads_.clear();
        lock.unlock();

        // make sure we do not lock during join
        for( auto it = local.begin(); it != local.end(); ++it )
            (*it)->join();
    }
}
