// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SecurePool.h"
#include "Pool_ABC.h"
#include "cpplog/cpplog.hpp"

#include <boost/bind.hpp>

using namespace host;

// -----------------------------------------------------------------------------
// Name: SecurePool::SecurePool
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
SecurePool::SecurePool( cpplog::BaseLogger& log, const std::string& name, Pool_ABC& pool )
    : log_ ( log )
    , name_( name )
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
    // NOTHING
}

namespace
{
    void WrapTask( const Pool_ABC::Task& task, cpplog::BaseLogger& log, const std::string& name )
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
    }
}

// -----------------------------------------------------------------------------
// Name: SecurePool::Post
// Created: BAX 2012-04-16
// -----------------------------------------------------------------------------
void SecurePool::Post( const Pool_ABC::Task& task )
{
    pool_.Post( boost::bind( &::WrapTask, task, boost::ref( log_ ), name_ ) );
}
