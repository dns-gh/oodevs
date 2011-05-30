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
//#include <google/protobuf/text_format.h>
#include <iostream>

#include "PBTools.h"

namespace
{
    void PrintMessage(std::ostream& os, const google::protobuf::Message& msg)
    {
//        std::string str;
//        google::protobuf::TextFormat::PrintToString( msg, &str );
//        os << str << std::endl;
        launcher_test_app::PBTools::ToXML( os, msg );
    }
}
namespace launcher_test_app
{
// -----------------------------------------------------------------------------
// Name: ResponseHandler constructor
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
ResponseHandler::ResponseHandler( std::ostream& os, boost::mutex& mutex, boost::condition_variable& cond )
    : os_( os )
    , mutex_( mutex )
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
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionStartResponse& message )
{
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionStopResponse& message )
{
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::ProfileListResponse& message )
{
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionCommandExecutionResponse& message )
{
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionNotification& message )
{
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionParameterChangeResponse& message )
{
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-30
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionListResponse& message )
{
    PrintMessage( os_, message );
    Notify();
}
// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::SessionStatus& message )
{
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::ConnectedProfileListResponse& message )
{
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-30
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::CheckpointListResponse& message )
{
    PrintMessage( os_, message );
    Notify();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandler::Handle
// Created: AHC 2011-05-30
// -----------------------------------------------------------------------------
void ResponseHandler::Handle( const sword::CheckpointDeleteResponse& message )
{
    PrintMessage( os_, message );
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
