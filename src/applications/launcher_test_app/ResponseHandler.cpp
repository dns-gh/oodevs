// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_test_app_pch.h"
#include "ResponseHandler.h"
#include <boost/thread.hpp>
#include "protocol/LauncherSenders.h"

#include <iostream>
#include "PBTools.h"

namespace launcher_test_app
{
// -----------------------------------------------------------------------------
// Name: ResponseHandler constructor
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
ResponseHandler::ResponseHandler( boost::mutex& mutex, boost::condition_variable& cond )
    : mutex_( mutex )
    , cond_( cond )
    , messagesReceived_( 0 )
{
    // NOTHING;
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler destructor
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
ResponseHandler::~ResponseHandler()
{
    // NOTHING;
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::ExerciseListResponse& message )
{
    PBTools::ToXML(std::cout, message);
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionStartResponse& message )
{
    PBTools::ToXML(std::cout, message);
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionStopResponse& message )
{
    PBTools::ToXML(std::cout, message);
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::ProfileListResponse& message )
{
    PBTools::ToXML(std::cout, message);
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionCommandExecutionResponse& message )
{
    PBTools::ToXML(std::cout, message);
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionNotification& message )
{
    PBTools::ToXML(std::cout, message);
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionParameterChangeResponse& message )
{
    PBTools::ToXML(std::cout, message);
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionStatus& message )
{
    PBTools::ToXML(std::cout, message);
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::ConnectedProfileListResponse& message )
{
    PBTools::ToXML(std::cout, message);
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Notify
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Notify()
{
    {
        boost::mutex::scoped_lock lock(mutex_);
        ++messagesReceived_;
    }
    cond_.notify_all();
}

}
