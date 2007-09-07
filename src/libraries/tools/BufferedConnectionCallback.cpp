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
BufferedConnectionCallback::Event::Event( const std::string& endpoint )
    : endpoint_( endpoint )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedConnectionCallback::Event
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
BufferedConnectionCallback::Event::Event( const std::string& address, const std::string& error )
    : endpoint_( address )
    , error_( error )
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
    for( CIT_Events it = events.begin(); it != events.end(); ++it )
    {
        const Event& event = *it;
        if( ! event.error_.empty() )
            callback.ConnectionFailed( event.endpoint_, event.error_ );
        else
            callback.ConnectionSucceeded( event.endpoint_ );
    }
}

// -----------------------------------------------------------------------------
// Name: BufferedConnectionCallback::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedConnectionCallback::ConnectionSucceeded( const std::string& endpoint )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedConnectionCallback::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedConnectionCallback::ConnectionFailed( const std::string& address, const std::string& error )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( address, error ) );
}
