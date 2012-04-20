// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SecurePool.h"

#include "cpplog/cpplog.hpp"
#include "Pool_ABC.h"

#include <boost/bind.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;

class host::WaitGroup : public boost::noncopyable
{
public:
    WaitGroup()
        : counter_( 0 )
    {
        // NOTHING
    }

    ~WaitGroup()
    {
        // NOTHING
    }

    void Add()
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        counter_++;
    }

    void Done()
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        counter_--;
        condition_.notify_one();
    }

    void Join()
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        while( counter_ > 0 )
            condition_.wait( lock );
    }

private:
    boost::mutex access_;
    boost::condition_variable condition_;
    int counter_;
};

// -----------------------------------------------------------------------------
// Name: SecurePool::SecurePool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
SecurePool::SecurePool( cpplog::BaseLogger& log, const std::string& name, Pool_ABC& pool )
    : log_ ( log )
    , name_( name )
    , wait_( new WaitGroup() )
    , pool_( pool )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SecurePool::~SecurePool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
SecurePool::~SecurePool()
{
    wait_->Join();
}

namespace
{
void WrapTask( WaitGroup& wait, const Pool_ABC::Task& task, cpplog::BaseLogger& log, const std::string& name )
{
    try
    {
        task();
    }
    catch( const std::exception& err )
    {
        LOG_ERROR( log ) << "[" << name << "] " << err.what();
    }
    catch( ... )
    {
        LOG_ERROR( log ) << "[" << name << "] Unexpected exception";
    }
    wait.Done();
}
}

// -----------------------------------------------------------------------------
// Name: SecurePool::Post
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
void SecurePool::Post( const Pool_ABC::Task& task )
{
    wait_->Add();
    pool_.Post( boost::bind( &::WrapTask, boost::ref( *wait_ ), task, boost::ref( log_ ), name_ ) );
}
