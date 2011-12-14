// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "BufferedSocketEventCallback.h"
#include <boost/bind.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: BufferedSocketEventCallback constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
BufferedSocketEventCallback::BufferedSocketEventCallback()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedSocketEventCallback destructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
BufferedSocketEventCallback::~BufferedSocketEventCallback()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BufferedSocketEventCallback::OnMessage
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedSocketEventCallback::OnMessage( const std::string& endpoint, Message& message )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( boost::bind( &BufferedSocketEventCallback::OnMessageWrapper, this, _1, endpoint, message ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedSocketEventCallback::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedSocketEventCallback::ConnectionSucceeded( const std::string& endpoint )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( boost::bind( &SocketEventCallback_ABC::ConnectionSucceeded, _1, endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedSocketEventCallback::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedSocketEventCallback::ConnectionFailed( const std::string& endpoint, const std::string& error )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( boost::bind( &SocketEventCallback_ABC::ConnectionFailed, _1, endpoint, error ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedSocketEventCallback::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedSocketEventCallback::ConnectionError( const std::string& endpoint, const std::string& error )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( boost::bind( &SocketEventCallback_ABC::ConnectionError, _1, endpoint, error ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedSocketEventCallback::ConnectionWarning
// Created: MCO 2011-09-26
// -----------------------------------------------------------------------------
void BufferedSocketEventCallback::ConnectionWarning( const std::string& endpoint, const std::string& warning )
{
    boost::mutex::scoped_lock locker( mutex_ );
    events_.push_back( boost::bind( &SocketEventCallback_ABC::ConnectionWarning, _1, endpoint, warning ) );
}

// -----------------------------------------------------------------------------
// Name: BufferedSocketEventCallback::Commit
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void BufferedSocketEventCallback::Commit( SocketEventCallback_ABC& callback )
{
    T_Events events;
    {
        boost::mutex::scoped_lock locker( mutex_ );
        std::swap( events_, events );
    }
    for( IT_Events it = events.begin(); it != events.end(); ++it )
        (*it)( callback );
}


// -----------------------------------------------------------------------------
// Name: BufferedSocketEventCallback::OnMessageWrapper
// Created: MCO 2010-12-07
// -----------------------------------------------------------------------------
void BufferedSocketEventCallback::OnMessageWrapper( SocketEventCallback_ABC& callback, const std::string& endpoint, Message& message ) const
{
    try
    {
        callback.OnMessage( endpoint, message );
    }
    catch( std::exception& e )
    {
        callback.ConnectionWarning( endpoint, e.what() );
    }
}
