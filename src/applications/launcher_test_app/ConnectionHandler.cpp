// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


#include "launcher_test_app_pch.h"
#include "ConnectionHandler.h"
#include <boost/thread.hpp>

namespace launcher_test_app
{
// -----------------------------------------------------------------------------
// Name: ConnectionHandler constructor
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
ConnectionHandler::ConnectionHandler( boost::mutex& mutex, boost::condition_variable& cond )
    : connected_( false )
    , mutex_( mutex )
    , cond_( cond )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandler destructor
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
ConnectionHandler::~ConnectionHandler()
{
    // NOTHINF
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandler::OnConnectionSucceeded
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ConnectionHandler::OnConnectionSucceeded()
{
    {
        boost::mutex::scoped_lock lock(mutex_);
        connected_ = true;
    }
    cond_.notify_all();
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandler::OnConnectionFailedr
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ConnectionHandler::OnConnectionFailed( const std::string& reason )
{
    {
        boost::mutex::scoped_lock lock(mutex_);
        connected_ = false;
        error_ = reason;
    }
    cond_.notify_all();
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandler::OnConnectionLost
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ConnectionHandler::OnConnectionLost( const std::string& reason )
{
    {
        boost::mutex::scoped_lock lock(mutex_);
        connected_ = false;
        error_ = reason;
    }
    cond_.notify_all();
}

// -----------------------------------------------------------------------------
// Name: ConnectionHandler::OnError
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ConnectionHandler::OnError( const std::string& message )
{
    {
        boost::mutex::scoped_lock lock(mutex_);
        connected_ = false;
        error_ = message;
    }
    cond_.notify_all();
}
}
