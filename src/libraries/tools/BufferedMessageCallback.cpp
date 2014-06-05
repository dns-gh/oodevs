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
#include "ConnectionError.h"

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
BufferedMessageCallback::Event::Event( const std::string& local,
    const std::string& remote, const std::string& error, const std::string& warning,
    bool connection )
    : local_( local )
    , remote_( remote )
    , error_( error )
    , warning_( warning )
    , connection_( connection )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::Event::Event
// Created: AGE 2007-09-07
// -----------------------------------------------------------------------------
BufferedMessageCallback::Event::Event( const std::string& remote, const Message& message )
    : remote_( remote )
    , message_( message )
    , connection_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::OnError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedMessageCallback::OnError( const std::string& remote, const std::string& error )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( "", remote, error, "", false ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::OnWarning
// Created: MCO 2011-09-26
// -----------------------------------------------------------------------------
void BufferedMessageCallback::OnWarning( const std::string& remote, const std::string& warning )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( "", remote, "", warning, false ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::OnMessage
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedMessageCallback::OnMessage( const std::string& remote, Message& message )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( remote, message ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedMessageCallback::ConnectionSucceeded( const std::string& local,
        const std::string& remote )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( local, remote, "", "", true ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedMessageCallback::ConnectionFailed( const std::string& address, const std::string& error )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( Event( "", address, error, "", true ) );
}
// -----------------------------------------------------------------------------
// Name: BufferedMessageCallback::Commit
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedMessageCallback::Commit( ConnectionCallback_ABC& connection, 
            MessageCallback_ABC& message )
{
    T_Events events;
    {
        boost::mutex::scoped_lock locker( mutex_ );
        std::swap( events_, events );
    }
    for( IT_Events it = events.begin(); it != events.end(); ++it )
    {
        if( it->connection_ )
        {
            // Connection event
            if( ! it->error_.empty() )
                connection.ConnectionFailed( it->remote_, it->error_ );
            else
                connection.ConnectionSucceeded( it->local_, it->remote_ );
        }
        else
        {
            // Message event
            if( ! it->error_.empty() )
                message.OnError( it->remote_, it->error_ );
            else if( ! it->warning_.empty() )
                message.OnWarning( it->remote_, it->warning_ );
            else
                Commit( message, it->remote_, it->message_ );
        }
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
    catch( const DisconnectionRequest& e )
    {
        callback.OnError( endpoint, tools::GetExceptionMsg( e ) );
    }
    catch( const ConnectionError& e )
    {
        callback.OnError( endpoint, tools::GetExceptionMsg( e ) );
        throw;
    }
    catch( const std::exception& e )
    {
        callback.OnWarning( endpoint, tools::GetExceptionMsg( e ) );
    }
}
