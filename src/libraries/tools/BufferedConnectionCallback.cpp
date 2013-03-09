// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "BufferedConnectionCallback.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: BufferedConnectionCallback constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
BufferedConnectionCallback::BufferedConnectionCallback()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedConnectionCallback destructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
BufferedConnectionCallback::~BufferedConnectionCallback()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedConnectionCallback::Event
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
BufferedConnectionCallback::Event::Event( const std::string& local,
        const std::string& remote, const std::string& error )
    :   local_( local )
    ,   remote_( remote )
    ,   error_( error )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedConnectionCallback::Commit
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedConnectionCallback::Commit( ConnectionCallback_ABC& callback )
{
    T_Events events;
    {
        boost::mutex::scoped_lock locker( mutex_ );
        std::swap( events, events_ );
    }
    for( auto it = events.begin(); it != events.end(); ++it )
    {
        const Event& event = *it;
        if( ! event.error_.empty() )
            callback.ConnectionFailed( event.remote_, event.error_ );
        else
            callback.ConnectionSucceeded( event.local_, event.remote_ );
    }
}

// -----------------------------------------------------------------------------
// Name: BufferedConnectionCallback::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedConnectionCallback::ConnectionSucceeded( const std::string& local,
        const std::string& remote )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( local, remote, "" ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedConnectionCallback::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedConnectionCallback::ConnectionFailed( const std::string& address, const std::string& error )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( "", address, error ) );
}
