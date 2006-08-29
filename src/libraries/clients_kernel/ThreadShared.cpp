// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ThreadShared.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ThreadShared constructor
// Created: AGE 2006-08-28
// -----------------------------------------------------------------------------
ThreadShared::ThreadShared()
    : count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ThreadShared destructor
// Created: AGE 2006-08-28
// -----------------------------------------------------------------------------
ThreadShared::~ThreadShared()
{
    Join();
}

namespace
{
    struct is_null
    {
        explicit is_null( const volatile unsigned& value ) : value_( &value ) {};
        bool operator()() const { return *value_ == 0; };
    private:
        const volatile unsigned* value_;
    };
}

// -----------------------------------------------------------------------------
// Name: ThreadShared::Join
// Created: AGE 2006-08-28
// -----------------------------------------------------------------------------
void ThreadShared::Join() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    condition_.wait( locker, is_null( count_ ) );
}

// -----------------------------------------------------------------------------
// Name: ThreadShared::Locker
// Created: AGE 2006-08-28
// -----------------------------------------------------------------------------
ThreadShared::Locker::Locker( const ThreadShared& shared )
    : shared_( &shared )
{
    shared_->AddLock();
}

// -----------------------------------------------------------------------------
// Name: ThreadShared::Locker
// Created: AGE 2006-08-28
// -----------------------------------------------------------------------------
ThreadShared::Locker::Locker( const Locker& rhs )
    : shared_( rhs.shared_ )
{
    shared_->AddLock();
}
              
// -----------------------------------------------------------------------------
// Name: ThreadShared::~Locker
// Created: AGE 2006-08-28
// -----------------------------------------------------------------------------
ThreadShared::Locker::~Locker()
{
    shared_->RemoveLock();
}

// -----------------------------------------------------------------------------
// Name: ThreadShared::AddLock
// Created: AGE 2006-08-28
// -----------------------------------------------------------------------------
void ThreadShared::AddLock() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    ++count_;
}

// -----------------------------------------------------------------------------
// Name: ThreadShared::RemoveLock
// Created: AGE 2006-08-28
// -----------------------------------------------------------------------------
void ThreadShared::RemoveLock() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    --count_;
    condition_.notify_one();
}
