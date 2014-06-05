// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "ThreadPool.h"

#ifdef _MSC_VER
#   pragma warning( push, 0 )
#   pragma warning( disable : 4702 )
#endif
#include <boost/thread.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

#include <boost/asio.hpp>
#include <boost/make_shared.hpp>

using namespace tools;

namespace
{
    typedef boost::packaged_task< void > Package;

    struct Handler
    {
        explicit Handler( const ThreadPool::Task& task )
            : ptr_( boost::make_shared< Package >( task ) )
        {
            // NOTHING
        }
        void operator()() const
        {
            (*ptr_)();
        }
        boost::shared_ptr< Package > ptr_;
    };
}

struct ThreadPool::Private
{
    explicit Private( size_t numThreads )
        : service( numThreads )
        , work   ( new boost::asio::io_service::work( service ) )
    {
        // NOTHING
    }
    boost::thread_group threads;
    boost::asio::io_service service;
    std::unique_ptr< boost::asio::io_service::work > work;
};

// -----------------------------------------------------------------------------
// Name: ThreadPool::ThreadPool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
ThreadPool::ThreadPool( size_t numThreads )
    : private_( new Private( numThreads ) )
{
    for( size_t i = 0; i < numThreads; ++i )
        private_->threads.create_thread( boost::bind(
            &boost::asio::io_service::run, &private_->service ) );
}

// -----------------------------------------------------------------------------
// Name: ThreadPool::~ThreadPool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
ThreadPool::~ThreadPool()
{
    Join( ThreadPool::SkipPending );
}

// -----------------------------------------------------------------------------
// Name: ThreadPool::Promise
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
ThreadPool::Future ThreadPool::Promise( const Task& task )
{
    Handler handler( task );
    private_->service.post( handler );
    return Future( handler.ptr_->get_future() );
}

// -----------------------------------------------------------------------------
// Name: ThreadPool::Post
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
void ThreadPool::Post( const Task& task )
{
    private_->service.post( Handler( task ) );
}

// -----------------------------------------------------------------------------
// Name: ThreadPool::Join
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
void ThreadPool::Join( JoinType type )
{
    if( type == ThreadPool::ProcessPending )
        private_->work.reset();
    else
        private_->service.stop();
    private_->threads.join_all();
}
