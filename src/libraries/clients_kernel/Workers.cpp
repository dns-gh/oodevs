// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Workers.h"
#include "WorkerTask_ABC.h"
#undef Yield
#include "tools/thread/ThreadPool.h"
#include <windows.h>

using namespace kernel;

namespace
{
    struct PrioritySetter
    {
        void operator()()
        {
            SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_LOWEST );
            tools::thread::Thread::Sleep( 10000 );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Workers constructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Workers::Workers()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Workers destructor
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
Workers::~Workers()
{
    // NOTHING
}

namespace
{
    struct Adaptor
    {
        Adaptor( WorkerTask_ABC& task, boost::mutex& mutex, std::vector< WorkerTask_ABC* >& tasks )
            : task_ ( &task  )
            , mutex_( &mutex )
            , tasks_( &tasks ) {}
        void operator()()
        {
            task_->Process();
            boost::mutex::scoped_lock locker( *mutex_ );
            tasks_->push_back( task_ );
        };
        WorkerTask_ABC*                 task_;
        boost::mutex*                   mutex_;
        std::vector< WorkerTask_ABC* >* tasks_;
    };
}

// -----------------------------------------------------------------------------
// Name: Workers::Enqueue
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Workers::Enqueue( std::unique_ptr< WorkerTask_ABC > task )
{
    if( pool_.get() )
        pool_->Enqueue( Adaptor( *task.release(), mutex_, finished_ ) );
}

// -----------------------------------------------------------------------------
// Name: Workers::CommitTasks
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Workers::CommitTasks()
{
    T_Tasks toDeal;
    {
        boost::mutex::scoped_lock locker( mutex_ );
        std::swap( toDeal, finished_ );
    }

    for( auto it = toDeal.begin(); it != toDeal.end(); ++it )
    {
        (*it)->Commit();
        delete *it;
    }
}

// -----------------------------------------------------------------------------
// Name: Workers::Terminate
// Created: LGY 2014-04-09
// -----------------------------------------------------------------------------
void Workers::Terminate()
{
    pool_.reset();
    finished_.clear();
}

// -----------------------------------------------------------------------------
// Name: Workers::Initialize
// Created: LGY 2014-04-09
// -----------------------------------------------------------------------------
void Workers::Initialize()
{
    pool_.reset( new tools::thread::ThreadPool() );
    const auto threads = pool_->GetCpuNumber();
    for( auto i = 0u; i < threads; ++i )
        pool_->Enqueue( PrioritySetter() );
}
