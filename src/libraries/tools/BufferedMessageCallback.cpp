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
BufferedMessageCallback::Event::Event( const std::string& endpoint, const std::string& error, const std::string& warning )
    : endpoint_( endpoint )
    , error_   ( error )
    , warning_ ( warning )
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
    events_.push_back( Event( endpoint, error, "" ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::OnWarning
// Created: MCO 2011-09-26
// -----------------------------------------------------------------------------
void BufferedMessageCallback::OnWarning( const std::string& endpoint, const std::string& warning )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( endpoint, "", warning ) );
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
        else if( ! it->warning_.empty() )
            callback.OnWarning( it->endpoint_, it->warning_ );
        else
            Commit( callback, it->endpoint_, it->message_ );
    }
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::Commit
// Created: MCO 2010-12-07
// -----------------------------------------------------------------------------
void BufferedMessageCallback::Commit( MessageCallback_ABC& callback, const std::string& endpoint, Message& message ) const
{
    try
    {
        callback.OnMessage( endpoint, message );
    }
    catch( std::exception& e )
    {
        callback.OnWarning( endpoint, e.what() );
    }
    catch( ... )
    {
        callback.OnWarning( endpoint, "unknown exception" );
    }
}
