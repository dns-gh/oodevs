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

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;

namespace
{
bool CheckFuture( cpplog::BaseLogger& log, const Pool_ABC::Future& future, const std::string& name )
{
    if( !future.is_ready() )
        return false;
    if( future.has_exception() )
        LOG_ERROR( log ) << "[" << name << "] Unexpected exception";
    return true;
}
}

// -----------------------------------------------------------------------------
// Name: SecurePool::SecurePool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
SecurePool::SecurePool( cpplog::BaseLogger& log, const std::string& name, Pool_ABC& pool )
    : log_   ( log )
    , name_  ( name )
    , access_( new boost::mutex() )
    , pool_  ( pool )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SecurePool::~SecurePool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
SecurePool::~SecurePool()
{
    boost::wait_for_all( futures_.begin(), futures_.end() );
    std::for_each( futures_.begin(), futures_.end(),
        boost::bind( &CheckFuture, boost::ref( log_ ), _1, name_ ) );
}

// -----------------------------------------------------------------------------
// Name: SecurePool::Post
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
void SecurePool::Post( const Pool_ABC::Task& task )
{
    Pool_ABC::Future future = pool_.Post( task );
    boost::lock_guard< boost::mutex > lock( *access_ );
    futures_.erase(
        std::remove_if( futures_.begin(), futures_.end(),
            boost::bind( &CheckFuture, boost::ref( log_ ), _1, name_ ) ),
        futures_.end() );
    futures_.push_back( future );
}

// -----------------------------------------------------------------------------
// Name: SecurePool::Get
// Created: BAX 2012-05-04
// -----------------------------------------------------------------------------
Pool_ABC& SecurePool::Get() const
{
    return pool_;
}