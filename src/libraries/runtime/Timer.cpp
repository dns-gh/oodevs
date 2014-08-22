// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Timer_ABC.h"

#include "Pool_ABC.h"
#include "Event.h"

#include <boost/thread/future.hpp>
#include <boost/make_shared.hpp>

using namespace runtime;

namespace
{
struct PrivateTimer : public Timer_ABC
{
    PrivateTimer( Pool_ABC& pool, const boost::posix_time::time_duration& timeout, const Task& task )
        : timeout_( timeout )
        , task_   ( task )
    {
        future_ = pool.Post( boost::bind( &PrivateTimer::Run, this ) );
    }

    ~PrivateTimer()
    {
        Stop();
    }

    void Run()
    {
        while( !end_.Wait( timeout_ ) )
            task_();
    }

    void Stop()
    {
        end_.Signal();
        future_.wait();
    }

    const boost::posix_time::time_duration timeout_;
    const runtime::Task task_;
    runtime::Event end_;
    Pool_ABC::Future future_;
};
}

Timer runtime::MakeTimer( Pool_ABC& pool, const boost::posix_time::time_duration& timeout, const Task& task )
{
    return boost::make_shared< PrivateTimer >( boost::ref( pool ), timeout, task );
}
