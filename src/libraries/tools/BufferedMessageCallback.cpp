// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "BufferedMessageCallback.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
BufferedMessageCallback::BufferedMessageCallback()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback destructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
BufferedMessageCallback::~BufferedMessageCallback()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::Event::Event
// Created: AGE 2007-09-07
// -----------------------------------------------------------------------------
BufferedMessageCallback::Event::Event( const std::string& endpoint, const std::string& error )
    : endpoint_( endpoint )
    , error_( error )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::Event::Event
// Created: AGE 2007-09-07
// -----------------------------------------------------------------------------
BufferedMessageCallback::Event::Event( const std::string& endpoint, const Message& message )
    : endpoint_( endpoint )
    , message_ ( message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::OnError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedMessageCallback::OnError( const std::string& endpoint, const std::string& error )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( endpoint, error ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::OnMessage
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedMessageCallback::OnMessage( const std::string& endpoint, Message& message )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( endpoint, message ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::Commit
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedMessageCallback::Commit( MessageCallback_ABC& callback )
{
    T_Events events;
    events.reserve( events_.size() );
    {
        boost::mutex::scoped_lock locker( mutex_ );
        std::swap( events_, events );
    }
    
    for( IT_Events it = events.begin(); it != events.end(); ++it )
    {
        if( ! it->error_.empty() )
            callback.OnError( it->endpoint_, it->error_ );
        else
            callback.OnMessage( it->endpoint_, it->message_ );
    }
}
